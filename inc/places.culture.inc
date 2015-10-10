<?

// Функции вывода мест
function _places(){
  $url = 'http://all.culture.ru/api/1.0/places';

  $decode = _decode($url);

  $items = array();
  foreach ($decode->places as $key => $value) {
    $items[] = l($value->name, 'places/'.$value->_id);
  }

  $pager = _pager($decode->total);

  return theme('item_list', array('items' => $items)) . $pager;
}

function _place($pid){
  //декодируем урл места
  $url = 'http://all.culture.ru/api/1.0/places/'.$pid;
  $decode = _decode($url, FALSE); //false - значит, что мы не применяем фильтр региона

  //вытягиваем переменные места
  $place = $decode->place;                            //определяем массив с самим местом
  $image = $place->image;                             //получаем картинку места
  $name = $place->name;                               //получаем название события
  $category = $place->category->name;                 //название категории события
  $X = $place->address->mapPosition->coordinates[0];      //координаты
  $Y = $place->address->mapPosition->coordinates[1];
  $street = $place->address->street;                      //улица
  //если есть, то комментарий, как добраться
  $address_comment = ($place->address->comment) ? ' ('.$place->address->comment.')' : '' ;

  $output = theme('place', 
    array(
      'title' => $name,
      'address' => $place->address->street,
      'image' => '<img src="http://all.culture.ru/uploads/'.$image->name.'" width="300px" />',
      'description' => $place->description,
      'organization' => l($place->organization->name, 'organizations/'.$place->organization->_id),
      'category' => l($category, 'category/'.$place->category->_id),
      'tags' => _tags($place->tags),
      'locale' => l($place->locale->name, 'locale/'.$place->locale->_id),
      'contacts' => _contacts($place->contacts),
      'map' => _map($X, $Y, $name, $street, $address_comment),
      'schedule' => isset($place->workingSchedule) ? _schedule($place->workingSchedule) : NULL,
      'gallery' => isset($place->gallery) ? _gallery($place->gallery) : NULL,
    )
  );

  $title[] = $name;
  $title[] = $category;
  drupal_set_title(implode(' - ', $title));

  return $output;
}