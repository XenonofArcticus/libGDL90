libGDL90
========

*UNDER VERY ACTIVE DEVELOPMENT!*

TODO
----

* Remove some of the "magic numbers"; for example, doing +2/-2 to
  account for the CRC bytes. In fact, the GDL90_ID_*_SIZE should
  probably include these bytes afterall, especially considering
  the CRC data can contain data escaped by the GDL90 "byte-stuffing"
  technique.
* Complete data query API.
* Add gdl90_error() and gdl90_error_str() functions.
* Add gdl90_setup(), allowing the programmer to configure the
  library in various way; particularly, allowing them to set
  the memory management/allocation model.
* Create a simple C++14 wrapper header.
