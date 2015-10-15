<?

// Функции вывода событий
function _events($region = TRUE, $search = ''){
  $url = 'http://all.culture.ru/api/1.0/events';
  $decode = _decode($url, $region, $search);

  $output = '';
  foreach ($decode->events as $key => $event) {
    $output .= theme('events', 
      array(
        'title' => l($event->name, 'events/'.$event->_id),
        'image' => _image($event->image->name, 'image'),
        'description' => truncate_utf8(strip_tags($event->description), 300, TRUE, TRUE),
        'price' => ($event->isFree) ? 'Событие бесплатно' : 'Событие платно. ' . _price($event->price, $event->maxPrice),
        'seance' => _seances_from_to($event->seances),
      )
    );
  }

  $pager = _pager($decode->total);
  $output .= $pager;

  return $output;
}

//выводит одно событие
function _event($eid){
  //декодируем урл события
  $url = 'http://all.culture.ru/api/1.0/events?ids='.$eid;
  $decode = _decode($url, FALSE); //false - значит, что мы не применяем фильтр региона

  if(sizeof($decode->events) == 0){
    return drupal_not_found();  
  }
  else{
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
        'seances' => _seances_all($event->seances),
        'price' => ($event->isFree) ? 'Событие бесплатно' : 'Событие платно. ' . _price($event->price, $event->maxPrice),
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


}

//функция, которая выводит сеансы
function _seances_all($seanses_array){
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

//функция, которая выводит сеансы с датой от и до..
function _seances_from_to($seanses_array){
  $output = '';
  if(sizeof($seanses_array) > 1){
    $first = array_shift($seanses_array);
    $end = end($seanses_array);
    $output = date('d.m.Y -', $first->start/1000) . date(' d.m.Y', $end->end/1000);
  }
  else{
    $seance = array_shift($seanses_array);
    $start = $seance->start/1000;
    $end = $seance->end/1000;
    $output = date('d.m.Y, H:i', $start) . date(' - H:i', $end);
  }
  return $output; 
}

//вывод экстра-полей
function _extra($extra_array){
  $output = 'Персоны: <br>';
  $person_type = array(
    'director' => 'Директор: ',
    'actor' => 'Актёры: ',
    'artisticDirector' => 'Художественный руководитель: ',
    'choreographer' => 'Хореограф: ',
    'conductor' => 'Дерижёр: ',
  );
  $items = array();
  foreach ($extra_array as $key => $person) {
    $items[$person->role][] = $person->name; 
  }
  foreach ($items as $role => $name_list) {
    if(isset($person_type[$role])){
      $output .= $person_type[$role] . theme('item_list', array('items' => $name_list));
    }
    else{
      $output .= theme('item_list', array('items' => $name_list));
      dsm($role);
    }
  }

  return $output;
}

function _price($price, $maxprice){
  $output = '';
  if($price == $maxprice){
    $output = 'Стоимость: ' . $price . ' руб.';
  }
  else {
    $output = 'Стоимость: ' . $price . ' - ' . $maxprice . ' руб.';
  }
  return $output;
}