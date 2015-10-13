<?

// Функции вывода мест
function _main_sitemap(){
  $domain = $GLOBALS['base_url'];
  print $domain . "/places.txt\n";
  print $domain . "/events.txt\n";
  print $domain . "/organizations.txt\n";
}

function _events_sitemap(){
  $end = _decode('http://all.culture.ru/api/1.0/events?sort=-_id&limit=1', FALSE)->events[0]->_id; //false - значит, что мы не применяем фильтр региона
  $start = _decode('http://all.culture.ru/api/1.0/events?sort=_id&limit=1', FALSE)->events[0]->_id;
  $domain = $GLOBALS['base_url'];

  for ($i = $start; $i <= $end; $i++) { 
    echo $domain . '/events/' . $i . "\n";
  }
}

function _places_sitemap(){
  $end = _decode('http://all.culture.ru/api/1.0/places?sort=-_id&limit=1', FALSE)->places[0]->_id; //false - значит, что мы не применяем фильтр региона
  $start = _decode('http://all.culture.ru/api/1.0/places?sort=_id&limit=1', FALSE)->places[0]->_id;
  $domain = $GLOBALS['base_url'];

  for ($i = $start; $i <= $end; $i++) { 
    echo $domain . '/places/' . $i . "\n";
  }
}

function _organizations_sitemap(){
  $end = _decode('http://all.culture.ru/api/1.0/organizations?sort=-_id&limit=1', FALSE)->organizations[0]->_id; //false - значит, что мы не применяем фильтр региона
  $start = _decode('http://all.culture.ru/api/1.0/organizations?sort=_id&limit=1', FALSE)->organizations[0]->_id;
  $domain = $GLOBALS['base_url'];

  for ($i = $start; $i <= $end; $i++) { 
    echo $domain . '/organizations/' . $i . "\n";
  }
}