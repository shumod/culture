<?php

//возвращает список регионов
function _regions(){	
	$regions = array();
	$regions = _get_regions_list('list');

	return theme('item_list', array('items' => $regions));
}

//возвращает регион
function _region($region_url){	
	$output = '';
	dsm($region_url);
	$output .= theme('region', 
		array(
		  'title' => 'test',
		  'places' => l('Места', 'r/' . $region_url . '/places'),
		  'events' => l('События', 'r/' . $region_url . '/events'),
		)
	);

	return $output;
}

function _region_title($region_url){
	$title = _get_region_term_from_url($region_url)->name;
	return $title;
}