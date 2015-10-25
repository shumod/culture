<?

module_load_include('inc', 'culture', 'inc/places.culture');
module_load_include('inc', 'culture', 'inc/events.culture');

function _search_page(){
	$output = 'Информации по заданным условиям не найдено';
	$help = '<div id="search-help">Поиск по событиям и местам производится по части слова или целому слову, которое содержится в названии. Поиск ведётся без учёта региона!</div>';

	//задаём заголовок
	$title = (isset($_GET['type'])) ? _get_search_title($_GET['type']) : 'Поиск' ;
	drupal_set_title($title);

	//ищем по тексту
	$search_text = (isset($_GET['text'])) ? $_GET['text'] : FALSE ;
	if($search_text){
		$search_type = (isset($_GET['type'])) ? $_GET['type'] : FALSE;
		switch ($search_type) {
			case 'Места':
				$output = _places(FALSE, $search_text);
				break;
			case 'События':
				$output = _events(FALSE, $search_text);
				break;
		}
	}
	return $help . $output;
}

function _get_search_title($type = ''){
	$title = '';
	if($type != ''){
		if($type == 'Места'){
			$title = 'Поиск по местам';
		}
		elseif($type == 'События'){
			$title = 'Поиск по событиям';
		}
		else{
			$title = 'Поиск';
		}
	}

	return $title;
}

//форма поиска
function _search_form($form, &$form_state){
  $form = array();

  $default_text = (isset($_GET['text'])) ? $_GET['text'] : '';
  $form['search-text'] = array(
    '#type' => 'textfield', 
    '#title' => 'Что ищем?', 
    '#size' => 20, 
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