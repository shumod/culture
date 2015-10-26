<?

//форма для вывода локалей
function locales_form($form, &$form_state){

  $form = array();

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Начать обновление',
  );

  $total_url = 'http://all.culture.ru/api/1.0/locales?limit=1';
  $total = _decode($total_url, FALSE)->total;

  $terms_array = taxonomy_get_tree(taxonomy_vocabulary_machine_name_load('locales')->vid);
  $terms_count = sizeof($terms_array);

  $form['markup'] = array(
    '#markup' => 'Всего локалей в открытой базе: ' . $total . ', в нашей базе: ' . $terms_count,
  );

  $morpher_url = 'http://api.morpher.ru/WebService.asmx/GetXml?s=слово';
  $options = array(
    'method' => 'GET',
    'timeout' => 15,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );
  $result = drupal_http_request($morpher_url, $options);
  dsm($result);
  $xml = simplexml_load_string($result->data);
  dsm($xml); 

  return $form;
}

//при клике на кнопку с локалями - создавать batch
function locales_form_submit($form, &$form_state){

  $vid = taxonomy_vocabulary_machine_name_load('locales')->vid;

  //формируем массив всех локалей. за один раз получать можно только 1000 локалей
  $total_url = 'http://all.culture.ru/api/1.0/locales?limit=1';

  $total = _decode($total_url, FALSE)->total;

  $operations = array();
  for($i = 0; $i < $total; $i++){
    $operations[] = array('reload_taxonomy_locale_term', array($vid));
  }

  $batch = array(
    // Массив операций и их параметров
    'operations' => $operations,
    // Функция, которая будет выполнена после окончания всех операций
    'finished' => 'locale_batch_finished',
    // Заголовок страницы с прогресс баром.
    // Опционально, по умолчанию t('Processing')
    'title' => 'Обновление терминов таксономии',
    // Сообщение, показываемое при инициализации.
    // Опционально, по умолчанию t('Initializing.')
    'init_message' => 'Подготовка данных',
    // Сообщение, показывемое при выполнении операций.
    // Опционально, по умолчанию t('Completed @current of @total.')
    'progress_message' => 'Выполнено @current из @total.',
    // Сообщение показываемое при ошибке выполнения операции.
    // Опционально, по умолчанию t('An error has occurred.')
    'error_message' => 'Произошла ошибка.',
  );

  batch_set($batch);

}


//функция либо обновляет термин, либо добавляет новый
function reload_taxonomy_locale_term($vid, &$context) {

  $offset = sizeof($context['results']);

  $url = 'http://all.culture.ru/api/1.0/locales?limit=1&offset='.$offset;
  $options = array(
    'method' => 'GET',
    'timeout' => 15,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );
  $result = drupal_http_request($url, $options);
  $decode = json_decode($result->data);

  $parent = 0;

  $term = array(
    'vid' => $vid,
    'name' => $decode->locales[0]->name,
  );

  $term = (object) $term;

  $term_id = db_select('field_data_field_locales_id', 'lid')
    ->fields('lid', array('entity_id'))
    ->condition('lid.field_locales_id_value', $decode->locales[0]->_id)
    ->execute()
    ->fetchField();

  //записываем термин только в том случае, если такого ещё нет в базе данных
  if(!$term_id) {
    taxonomy_term_save($term);   

    $parent_id = db_select('field_data_field_locales_id', 'lid')
      ->fields('lid', array('entity_id'))
      ->condition('lid.field_locales_id_value', $decode->locales[0]->parentId)
      ->execute()
      ->fetchField();
    $parent = ($parent_id == '') ? 0 : $parent_id;
    
    $term->field_locales_id['und'][0]['value'] = $decode->locales[0]->_id;
    $term->field_locales_timezone['und'][0]['value'] = $decode->locales[0]->timezone;
    $term->parent[0] = $parent;
    taxonomy_term_save($term);
  }

  $context['results'][] = 'good';

}

//выводит сообщения, когда список операций закончен
function locale_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message('Затронуто ' . count($results) . ' материалов:');
    dsm($results);
  }
  else {
    drupal_set_message('Завершено с ошибками.', 'error');
  }
}