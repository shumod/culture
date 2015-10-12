<?

// Функции вывода мест
function _main_sitemap(){
  $domain = $GLOBALS['base_url'];
  print $domain . '/places.txt';
  print $domain . '/events.txt';
  print $domain . '/organizations.txt';
}

function _events_sitemap(){
  $end_event = _decode('http://all.culture.ru/api/1.0/events?sort=-_id&limit=1', FALSE)->events[0]->_id; //false - значит, что мы не применяем фильтр региона
  $start_event = _decode('http://all.culture.ru/api/1.0/events?sort=_id&limit=1', FALSE)->events[0]->_id;
  $domain = $GLOBALS['base_url'];

  for ($i = $start_event; $i < $end_event; $i++) { 
    echo $domain . '/events/' . $i . "\n";
  }
}