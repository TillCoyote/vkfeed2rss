# vkfeed2rss
Эта программа предназначена для конвертации лент ВКонтакте в RSS ленты. Находится в альфа стадии и нуждается в доработке. На данный момент способна на:
* Конвертацию постов сообществ и страниц в RSS
* Поддержка одной прикреплённой картинки

# Примеры работы
На сервере автора есть [обновляемая RSS лента](http://reedych.ddns.net/rss/lentach.rss).

### Сборка
Для начала загрузите копию исходников программы и проверьте зависимости:
* jansson 2.10+, но можно меньше
* libcurl 7.x
* clang, можно gcc отредактировав makefile, но всё же лучше clang

```
make
./vkfeed2rss # запуск
```
### Сопровождение программы
Программа была написана наскоряк за 2 дня и содержит ад, причём код ещё и внушительного размера. Прошу не прикосаться к коду до версии 1.0 если вы не мазохист.
В папке script/ находятся вспомогательные скрипты для тестов API и пр.
