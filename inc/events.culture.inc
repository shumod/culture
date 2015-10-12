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
  //декодируем урл события
  $url = 'http://all.culture.ru/api/1.0/events?ids='.$eid;
  $decode = _decode($url, FALSE); //false - значит, что мы не применяем фильтр региона

  //вытягиваем переменные события
  $event = $decode->events[0];                            //определяем массив с самим событием
  $image = $event->image;                                 //получаем картинку события
  $name = $event->name;                                   //получаем название события
  $place = ($event->place) ? $event->place->name : NULL;  //если есть место проведения, то место
  $category = $event->category->name;                     //название категории события
  $X = $event->address->mapPosition->coordinates[0];      //координаты
  $Y = $event->address->mapPosition->coordinates[1];     
  $street = $event->address->street;                      //улица
  //если есть, то комментарий, как добраться
  $address_comment = ($event->address->comment) ? ' ('.$event->address->comment.')' : '' ; 

  //это для вывода в .tpl файл
  $output = theme('event', 
    array(
      'title' => $name,
      'address' => $street.$address_comment,
      'description' => $event->description,
      'category' => l($category, 'category/'.$event->category->_id),
      'tags' => _tags($event->tags),
      'seances' => _seances($event->seances),
      'isfree' => ($event->isFree) ? 'Событие бесплатно' : 'Событие платно. ' . _price($event->price, $event->maxPrice),
      'persons' => (isset($event->extraFields->persons)) ? _extra($event->extraFields->persons) : '',
      'place' => ($event->place != NULL) ? l($event->place->name, 'places/'.$event->place->_id) : '',
      'map' => _map($X, $Y, $name, $street, $address_comment),
      'organization' => l($event->organization->name, 'organizations/'.$event->organization->_id),
      'gallery' => isset($event->gallery) ? _gallery($event->gallery) : NULL,
    )
  );

  //задаём тайтл для события
  $title[] = $name;
  $title[] = $category;
  if($place){ $title[] = $place; }
  drupal_set_title(implode(' - ', $title));

  //задаём мета-тэги
  $keywords_array = array();
  $keywords_array[] = $category;
  $keywords_array[] = strip_tags(_tags($event->tags));
  $description = (sizeof($event->shortDescription) > 0) ? strip_tags($event->shortDescription) : '' ;
  _set_seo_tags($keywords_array, $description);

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