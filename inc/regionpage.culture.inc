<?php

module_load_include('inc', 'culture', 'inc/places.culture');
module_load_include('inc', 'culture', 'inc/events.culture');

//возвращает список регионов
function _regions(){	
	$regions = array();
	$regions = _get_regions_list('list');

	return theme('item_list', array('items' => $regions));
}

//возвращает регион
function _region($region_url){	
	$output = '';
	$region = TRUE;
	$search = '';
	$limit = 3;
	$pager = FALSE;
	$_SESSION['region'] = arg(1);

	$output .= theme('regionpage', 
		array(
			'events' => _region_places($region, $search, $limit, $pager),
			'places' => _region_events($region, $search, $limit, $pager),
		)
	);	

	return $output;
}

function _region_places($region, $search, $limit, $pager){
	$output = '';

	$output .= '<h2>' . l('Все места', drupal_get_path_alias().'/places') . '</h2>';
	$output .= _places($region, $search, $limit, $pager);
	$output .= '<div class="more">' . l('Подробнее', drupal_get_path_alias().'/places') . '</div>';

	return $output;
}

function _region_events($region, $search, $limit, $pager){
	$output = '';

	$reg = _get_current_region();

	$output .= '<h2>' . l('Все события', drupal_get_path_alias().'/events') . '</h2>';
	$output .= _events($region, $search, $limit, $pager);
	$output .= '<div class="more">' . l('Подробнее', drupal_get_path_alias().'/events') . '</div>';

	return $output;
}