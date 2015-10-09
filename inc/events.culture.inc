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

  $decode = _decode($url, FALSE);

  $event = $decode->events[0];
  $image = $event->image;
  $output = theme('event', 
    array(
      'title' => $event->name,
      'address' => $event->address->street,
      'description' => $event->description,
      'category' => l($event->category->name, 'category/'.$event->category->_id),
      'tags' => _tags($event->tags),
      'seances' => _seances($event->seances),
      'isfree' => ($event->isFree) ? 'Событие бесплатно' : 'Событие платно. ' . _price($event->price, $event->maxPrice),
      'persons' => (isset($event->extraFields->persons)) ? _extra($event->extraFields->persons) : '',
      'place' => ($event->place != NULL) ? l($event->place->name, 'places/'.$event->place->_id) : '',
      'map' => _map($event->address->mapPosition->coordinates[0], $event->address->mapPosition->coordinates[1], $event->name, '<strong>'.$event->name.'</strong><br/>'.$event->address->street),
      'organization' => l($event->organization->name, 'organizations/'.$event->organization->_id),
      'gallery' => isset($event->gallery) ? _gallery($event->gallery) : NULL,
    )
  );

  return $output;
}

//функция, которая выводит сеансы
function _seances($seanses_array){
  $output = '';
  $seance = array();
  foreach ($seanses_array as $key => $value) {
    $start = $value->start/1000;
    $end = $value->end/1000;
    $seance[] = date('d.m.Y, H:i', $start) . date(' - H:i', $end);
  }
  $output = (sizeof($seance) > 0) ? 'Сеансы: ' . theme('item_list', array('items' => $seance)) : '' ;
  return $output; 
}

//вывод экстра-полей
function _extra($extra_array){
  $output = 'Персоны: <br>';
  $person_type = array(
    'director' => 'Директор:',
    'actor' => 'Актёры:',
    'artisticDirector' => 'Художественный руководитель',
  );
  $items = array();
  foreach ($extra_array as $key => $person) {
    $items[$person->role][] = $person->name; 
  }
  foreach ($items as $role => $name_list) {
    if(sizeof($name_list) > 1){
      $output .= $person_type[$role] . theme('item_list', array('items' => $name_list));
    }
    else{
      $output .= $person_type[$role] . $name_list[0] . '<br />';
    }
  }

  return $output;
}

function _price($price, $maxprice){
  return 'Стоимость: ' . $price . ' - ' . $maxprice . ' руб.';
}