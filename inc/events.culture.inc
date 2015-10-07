<?

// Функции вывода событий
function _events(){
  $url = 'http://all.culture.ru/api/1.0/events';

  $decode = _decode($url);

  $items = array();
  foreach ($decode->events as $key => $value) {
    $items[] = l($value->name, 'events/'.$value->_id);
  }

  $pager = _pager($decode->total);

  return theme('item_list', array('items' => $items)).$pager;
}

function _event($eid){
  $url = 'http://all.culture.ru/api/1.0/events?ids='.$eid;

  $decode = _decode($url);

  $event = $decode->events[0];
  $image = $event->image;
  $output = theme('event', 
    array(
      'title' => $event->name,
      'organization' => l($event->organization->name, 'organizations/'.$event->organization->_id),
    )
  );

  return $output;
}
