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
	$search_text = (isset($_GET['text'])) ? $_GET['text'] : FAKSE ;
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