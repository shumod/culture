<?

// Функции вывода организаций
function _organizations(){
  $url = 'http://all.culture.ru/api/1.0/organizations';

  $decode = _decode($url);

  $items = array();
  foreach ($decode->organizations as $key => $value) {
    $items[] = l($value->name, 'organizations/'.$value->_id);
  }

  $pager = _pager($decode->total);

  return theme('item_list', array('items' => $items)).$pager;
}

function _organization($oid){
  $url = 'http://all.culture.ru/api/1.0/organizations?ids='.$oid;

  $decode = _decode($url);

  $organization = $decode->organizations[0];
  $output = theme('organization', 
    array(
      'title' => $organization->name,
    )
  );

  return $output;
}