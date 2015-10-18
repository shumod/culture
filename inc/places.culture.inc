<?

// Функции вывода мест
function _places($region = TRUE, $search = ''){
  $url = 'http://all.culture.ru/api/1.0/places';
  $decode = _decode($url, $region, $search);

  $items = array();
  $output = '';
  if ($decode->total == 0) {
    $output = '<div class="not-found">Мест с заданными параметрами не найдено.</div>';
  }
  else{
    foreach ($decode->places as $key => $place) {
      $output .= theme('places', 
        array(
          'title' => l($place->name, 'places/' . $place->_id),
          'image' => _image($place->image->name, 'image'),
          'description' => truncate_utf8(strip_tags($place->description), 300, TRUE, TRUE),
        )
      );
    }

    $pager = _pager($decode->total);
    $output .= $pager;
  }
  return $output;
}

function _place($pid){
  //декодируем урл места
  $url = 'http://all.culture.ru/api/1.0/places/'.$pid;
  $decode = _decode($url, FALSE); //false - значит, что мы не применяем фильтр региона

  if(sizeof($decode->place) == 0){
    return drupal_not_found();  
  }
  else{
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
        'image' => _image($image->name),
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

    //добавляем ключевые слова
    $keywords_array = array();
    $keywords_array[] = $category;
    _set_seo_tags($keywords_array);

    return $output;
  }
}

function _seo_places($term_url){
  $url = drupal_lookup_path('source', $term_url);
  if($url){
    $args = explode('/', $url);
    dsm($args);
    return $url;    
  }
  else{
    return drupal_not_found();
  }
}