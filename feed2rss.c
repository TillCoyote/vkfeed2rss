#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <time.h>

#include "info.h"

int printf_nobr(const char *str) // вывод строки с заменой всех "<br>" на '\n'
{
	for (unsigned i = 0; i < strlen(str); i++) {
		if (str[i] == '<' && str[i+1] == 'b' && str[i+2] == 'r' && str[i+3] == '>') {
			printf("&lt;br&gt;");
			i += 3;
		}
		else
			putchar(str[i]);
	}
	return 0;
}

char *vremja() // время, аналог команды date
{
	time_t rawtime;
  struct tm * timeinfo;
  
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  return asctime (timeinfo);
}

int osnova_rss(char *zagolovok, char *opisanie, unsigned long long id, short tip) { // создаёт основу для rss и нужных данн
	printf("<?xml version=\"%s\"?>\n", XMLVERSION);
	printf("<rss version=\"%s\">\n", RSSVERSION);
	printf("\t<channel>\n");
	printf("\t\t<title>%s</title>\n", zagolovok);
	if (tip == 1) printf("\t\t<link>https://vk.com/club%llu</link>\n", id);
	else printf("\t\t<link>https://vk.com/id%llu</link>\n", id);
	printf("\t\t<description>%s</description>\n", opisanie);
	printf("\t\t<pubDate>%s</pubDate>\n", vremja());
	printf("\t\t<generator>%s</generator>\n", nazvanie);
	return 0;
}

int obrabotka(const char *lenta, const unsigned kolichestvo) // put - путь
{
	json_t *root;
	json_error_t error;
	
	root = json_loads(lenta, 0, &error);
	if (!root) {
		fprintf(stderr, "%s: произошла ошибка при обработке ленты на строке %d: %s", nazvanie, error.line, error.text);
		return -1;
	}

	json_t *response = json_object_get(root, "response");
	if(!json_is_object(root)) {
		fprintf(stderr, "%s: произошла неизвестная ошибка при обработке ленты, возможно превышен лимит запросов на сервера или неработоспособность интернета\n", nazvanie);
		return -1;
	}
	
	json_t *post;
	json_t *text;
	json_t *id;
	json_t *attachment;
	json_t *image;
	json_t *photoarray;
	json_t *is_pinned;
	json_t *attachment_type;
	for (unsigned i = 1; i <= kolichestvo; i++) {
		post       = json_array_get(response, i);
		text       = json_object_get(post, "text");
		id         = json_object_get(post, "id");
		is_pinned  = json_object_get(post, "is_pinned");
		attachment = json_object_get(post, "attachment"); // картинка
		attachment_type = json_object_get(attachment, "type");
		photoarray = json_object_get(attachment, "photo");
		image 		 = json_object_get(photoarray, "src_big");

		if (json_string_value(text) == NULL) break;
		else {
			printf("\t\t<item>\n");
			if (json_integer_value(is_pinned) == 0)
				printf("\t\t<title>Запись %lli</title>\n", json_integer_value(id));
			else if (json_integer_value(is_pinned) == 1)
				printf("\t\t<title>Прикреплённая запись %lli</title>\n", json_integer_value(id));
			printf("\t\t<description>");
			if (printf_nobr(json_string_value(text)) == -1) return -1; // эта строка, та, что выше и та, что ниже - сам пост, функция printf_nobr заменяет некоторые символы (которые может не понять читалка RSS) на помятные XML аналоги
			printf("</description>\n");
			if (json_is_string(image) == 1) /* если к записи была приложена картинка */ printf("\t\t<enclosure url=\"%s\" type=\"image/jpg\" />\n", json_string_value(image));
			printf("\t\t</item>\n");
		}
	}
	printf("\t</channel>\n");
	printf("</rss>\n");

	return 0;
}
