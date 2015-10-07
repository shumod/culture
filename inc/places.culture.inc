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
  $url = 'http://all.culture.ru/api/1.0/places/'.$pid;

  $decode = _decode($url);

  $place = $decode->place;
  $image = $place->image;
  $output = theme('place', 
    array(
      'title' => $place->name,
      'address' => $place->address->street,
      'image' => '<img src="http://all.culture.ru/uploads/'.$image->name.'" width="300px" />',
      'description' => $place->description,
      'organization' => l($place->organization->name, 'organizations/'.$place->organization->_id),
      'category' => l($place->category->name, 'category/'.$place->category->_id),
      'tags' => _tags($place->tags),
      'locale' => l($place->locale->name, 'locale/'.$place->locale->_id),
      'contacts' => _contacts($place->contacts),
      'map' => _map($place->address->mapPosition->coordinates[0], $place->address->mapPosition->coordinates[1], $place->name, '<strong>'.$place->name.'</strong><br/>'.$place->address->street),
      'schedule' => _schedule($place->workingSchedule),
    )
  );

  drupal_set_title($place->name);

  return $output;
}