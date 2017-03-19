[![Build Status](https://travis-ci.org/usox/hacore.svg?branch=master)](https://travis-ci.org/usox/hacore)

Hacore - Hack Config Reader
===========================

Hacore provides a simple approach to read json formatted config files in hack
strict mode.

Sample config
-------------

Hacore treats all config values as strings and will explicitly cast them. To
access the values of a single key, just use `getLeaf($key_name)`. To get a
complete branch of options, use `getBranch($key_name)`.

```json
{
	"foo":"bar",
	"barfoo":666,
	"more":{
		"config":"options"
	}
}
```

```php
$reader = new \Usox\Hacore\Reader();
$reader->load('path-to-config.json')

$reader->getLeaf('foo'); // returns 'bar'
$reader->getLeaf('barfoo'); // returns '666'
$reader->getBranch('more'); // returns a new Reader instance
```
