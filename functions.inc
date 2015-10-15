<?

//выводит форму с регионами из словаря
function _select_region_form($form, &$form_state){
  $form = array();

  $session = (isset($_SESSION['region'])) ? $_SESSION['region'] : 0 ;

  $form['#prefix'] = '<div id="reload-region">';
  $form['#suffix'] = '</div>';

  $form['region'] = array(
    '#type' => 'select',
    '#options' => _get_regions_list(),
    '#default_value' => $session,
    '#ajax' => array(
      'callback' => 'set_session',
      'wrapper' => 'reload-region',
    ),
  );
  $form['region-cookies'] = array(
    '#markup' => '<div id="region-cookies"></div>',
  );

  return $form;
}

//форма поиска
function _search_form($form, &$form_state){
  $form = array();

  $default_text = (isset($_GET['text'])) ? $_GET['text'] : '';
  $form['search-text'] = array(
    '#type' => 'textfield', 
    '#title' => 'Что ищем?', 
    '#size' => 30, 
    '#maxlength' => 128, 
    '#required' => TRUE,
    '#default_value' => $default_text,
  );

  $default_type = (isset($_GET['type'])) ? $_GET['type'] : 'События';
  $form['search-type'] = array(
    '#type' => 'radios',
    '#options' => drupal_map_assoc(array('События', 'Места')),
    '#default_value' => $default_type,
  );
  $form['search-submit'] = array(
    '#type' => 'submit',
    '#value' => 'Искать',       
    '#submit' => array('_search'),
  );

  return $form;
}

function _search($form, $form_state){
  $options = array();
  $options['query']['text'] = $form_state['values']['search-text'];
  $options['query']['type'] = $form_state['values']['search-type'];
  drupal_goto('search', $options);
}

function set_session($form, $form_state){
  $_SESSION['region'] = $form_state['input']['region'];
  $form['region-cookies'] = array(
    '#markup' => '<div id="region-cookies">Регион обновлён</div>',
  );  
  return $form;
}

function _ajax_command_page_reload() {
   return array(
    'command' => 'reload',
  );
}

//получаем список всех регионов
function _get_regions_list(){
  $vid = taxonomy_vocabulary_machine_name_load('locales')->vid;
  $term_rf = taxonomy_get_term_by_name('РФ');
  
  foreach ($term_rf as $value) {
    $tid_rf = $value->tid;
    $name_rf = 'Вся Россия';
  }
  $childrens = taxonomy_get_children($tid_rf, $vid);

  $output = array();
  $output[$tid_rf] = $name_rf;
  foreach ($childrens as $key => $value) {
    $output[$key] = $value->name;
  }

  return $output;
}

//выводит пейджер на страницу
function _pager($total){
  //pager
  $per_page = 20;
  $elements_count = $total;
  // Initialize the pager
  $current_page = pager_default_initialize($elements_count, $per_page);
  // Show the pager
  $pager = theme('pager', array('quantity', $elements_count));

  return $pager;
}

//функция декодирует урл и возвращает массив, с которым можно работать
function _decode($url, $region = TRUE, $search = ''){

  //функции для пейджера
  $data_arr = array();
  if(isset($_GET['page'])){
    $offset = $_GET['page']*20;
    $data_arr[] = 'offset='.$offset;
  }

  $term_rf = taxonomy_get_term_by_name('РФ');
  foreach ($term_rf as $value) {
    $tid_rf = $value->tid;
  }

  //если мы выводим информацию в соответствии с регионом
  if($region && isset($_SESSION['region'])){
    if ($_SESSION['region'] != $tid_rf) {
      $childrens = taxonomy_get_children($_SESSION['region']);
      
      $children = array();
      $children[] = $_SESSION['region'];
      foreach ($childrens as $key => $value) {
        $children[] = $value->field_locales_id['und'][0]['value'];
      }
      $data_arr[] = 'locales='.implode(',', $children);
    }
  }

  //если есть поиск, то добавляем в поиск параметры
  if($search != ''){
    $data_arr[] = 'nameQuery=' . $search;
  }

  //собираем массив параметров для запроса
  $data = (sizeof($data_arr) > 0) ? implode('&', $data_arr) : NULL;

  $options = array(
    'method' => 'GET',
    'data' => $data,
    'timeout' => 15,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );
  $result = drupal_http_request($url, $options);

  $decode = json_decode($result->data);

  dsm($decode);
  return $decode;
}

//формирует список тэгов
function _tags($tags_array){
  $tags = array();
  foreach ($tags_array as $key => $value) {
    $tags[] = l($value->name, 'tags/'.$value->_id);
  }
  $output = (sizeof($tags) > 0) ? implode(', ', $tags) : '';
  
  return $output;
}

//формирует список контактов из полученного массива
function _contacts($contacts_array){
  $items = array();

  //массив контактов, чтобы их называть
  $contact_name = array(
    'website' => 'Официальный сайт: ',
    'email' => 'E-mail: ',
    'vkontakte' => 'ВКонтакте: ',
    'odnoklassniki' => 'Одноклассники: ',
    'facebook' => 'Facebook: ',
    'twitter' => 'Twitter: ',
    'phones' => 'Телефоны: ',
  );

  //начинаем обход массива контактов
  foreach ($contacts_array as $key => $value) {

    if(is_array($value)){ //если контакт - это массив(например, несколько телефонов), то 
      $item = array();

      //перебираем массив контактов и делаем из него список
      foreach ($value as $contact) {
        $comment = ($contact->comment != '') ? ' ('.$contact->comment.')' : '';
        $item[] = _format_phone($contact->value) . $comment;
      }
      $items[] = $contact_name[$key].theme('item_list', array('items' => $item));
    }

    //если это просто одиночный контакт, то выводим его
    else{
      $items[] = $contact_name[$key].$value.'<br />';
    }

  }

  $output = (sizeof($items) > 0) ? 'Контакты: '.theme('item_list', array('items' => $items)) : '' ;

  return $output;
}

//выводим расписание работы
function _schedule($schedule){
  $output = '';
  $days = array('Понедельник', 'Вторник', 'Среда', 'Четверг', 'Пятница', 'Суббота', 'Воскресенье');
  $items = array();
  $minus = 9 * 60 * 60; //сколько вычесть часов, чтобы получить нормальное время
  foreach ($schedule as $key => $value) {
    $from = date("H:i", (int) $value->from / 1000 - 3*60*60);
    $to = date("H:i", (int) $value->to / 1000 - 3*60*60);

    $items[] = $days[$key] . ': '.$from. ' - ' . $to;
  }

  $output = (sizeof($items) > 0) ? 'Время работы: '.theme('item_list', array('items' => $items)) : '' ; 

  return $output;
}

//функция, которая отдаёт карту с точкой в заданных координатах
function _map($coordinateX, $coordinateY, $name, $street, $comment){
  drupal_add_js('https://api-maps.yandex.ru/2.1/?lang=ru_RU', 'external');
  $name = htmlspecialchars($name);
  $street = htmlspecialchars($street);
  $comment = htmlspecialchars($comment);
  drupal_add_js('
      ymaps.ready(init);
        var myMap,
            myPlacemark;

        function init(){     
            myMap = new ymaps.Map("map", {
                center: ['.$coordinateX.', '.$coordinateY.'],
                zoom: 12
            });

            myPlacemark = new ymaps.Placemark(['.$coordinateX.', '.$coordinateY.'], { 
                hintContent: "<strong>'.$name.'</strong>", 
                balloonContent: "<strong>'.$name.'</strong><br>'.$street.$comment.'",
            });

            myMap.geoObjects.add(myPlacemark);
        }
  ', array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
  $output = '';
  $output .= '<div id="coordinates">Координаты:<br /> ';
  $output .= '<span id="coordinate-x">X: '.$coordinateX.'</span><br />';
  $output .= '<span id="coordinate-y">Y: '.$coordinateY.'</span>';
  $output .= '</div>';

  $output .= '<div id="map" style="width: 600px; height: 400px"></div>';
  return $output;
}

//функция для вывода галлереи. получает массив и выводит обратно галерею для owl gallery
function _gallery($gallery){
  drupal_add_js('//cdn.jsdelivr.net/jquery.slick/1.5.7/slick.min.js', 'external');
  drupal_add_css('//cdn.jsdelivr.net/jquery.slick/1.5.7/slick.css', 'external');
  $image_width = 200;
  $image_height = 150;
  $output = '';

  if (sizeof($gallery) > 0) {
    $output = '<h2>Галерея:</h2> <div class="gallery">';
    foreach ($gallery as $foto) {
      //запрашиваем пресет фотографии
      $replace = '_w' . $image_width . '_h' . $image_height.'.';
      $foto_preset_url = str_replace('.', $replace, $foto->name);

      //получаем ссылку на обычную фотку
      $foto_url = 'http://all.culture.ru/uploads/' . $foto->name;

      //получаем тэг с фото
      $foto_preset = '<img src="http://all.culture.ru/uploads/'.$foto_preset_url.'" />';

      //формируем ссылку на фото
      $options = array(
        'html' => 'TRUE',
        'attributes' => array(
          'class' => array('colorbox'),
          'rel' => 'gallery',
        ),
      );
      $foto_link = l($foto_preset, $foto_url, $options);
      $output .= '<div>'.$foto_link.'</div>';
    }
    $output .= '</div>';
  }

  return $output;
}

//выводит картинку со ссылкой на колорбокс или обычную картинку
function _image($image_name, $type = 'colorbox', $width = 200, $height = 150){
  $image = '';

  $replace = '_w' . $width . '_h' . $height.'.';
  $preset_name = str_replace('.', $replace, $image_name);
  $preset = '<img src="http://all.culture.ru/uploads/'.$preset_name.'" />';

  if($type == 'colorbox'){


    $image_url = 'http://all.culture.ru/uploads/'.$image_name;

    //формируем ссылку на фото
    $options = array(
      'html' => 'TRUE',
      'attributes' => array(
        'class' => array('colorbox'),
        'rel' => 'gallery',
      ),
    );

    $image = l($preset, $image_url, $options);
  }
  elseif ($type == 'image') {
    $image = $preset;
  }

  return $image;
}

//добавляет тэги на страницу
function _set_seo_tags($keywords_array = array(), $description_string = ''){
  if(sizeof($keywords_array > 0)){
    $keywords = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'keywords',
        'content' => implode(',', $keywords_array),
      ),
    );
    drupal_add_html_head($keywords, 'meta_keywords');
  }

  if($description_string != ''){
    $description = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'description',
        'content' => $description_string,
      ),
    );
    drupal_add_html_head($description, 'meta_description');
  }
}

//форматирует номер телефона
function _format_phone($phone = '', $convert = false, $trim = true){
  // If we have not entered a phone number just return empty
  if (empty($phone)) {
    return '';
  }

  // Strip out any extra characters that we do not need only keep letters and numbers
  $phone = preg_replace("/[^0-9A-Za-z]/", "", $phone);

  // Do we want to convert phone numbers with letters to their number equivalent?
  // Samples are: 1-800-TERMINIX, 1-800-FLOWERS, 1-800-Petmeds
  if ($convert == true) {
    $replace = 
      array(
        '2'=>array('a','b','c'),
        '3'=>array('d','e','f'),
        '4'=>array('g','h','i'),
        '5'=>array('j','k','l'),
        '6'=>array('m','n','o'),
        '7'=>array('p','q','r','s'),
        '8'=>array('t','u','v'), 
        '9'=>array('w','x','y','z')
      );

    // Replace each letter with a number
    // Notice this is case insensitive with the str_ireplace instead of str_replace 
    foreach($replace as $digit=>$letters) {
      $phone = str_ireplace($letters, $digit, $phone);
    }
  }

  // If we have a number longer than 11 digits cut the string down to only 11
  // This is also only ran if we want to limit only to 11 characters
  if ($trim == true && strlen($phone)>11) {
    $phone = substr($phone,  0, 11);
  } 

  // Perform phone number formatting here
  if (strlen($phone) == 7) {
    return preg_replace("/([0-9a-zA-Z]{3})([0-9a-zA-Z]{4})/", "$1-$2", $phone);
  } elseif (strlen($phone) == 10) {
    return preg_replace("/([0-9a-zA-Z]{3})([0-9a-zA-Z]{3})([0-9a-zA-Z]{4})/", "($1) $2-$3", $phone);
  } elseif (strlen($phone) == 11) {
    return preg_replace("/([0-9a-zA-Z]{1})([0-9a-zA-Z]{3})([0-9a-zA-Z]{3})([0-9a-zA-Z]{4})/", "$1($2) $3-$4", $phone);
  }

  // Return original phone if not 7, 10 or 11 digits long
  return $phone;
}