<?

// Функции вывода событий
function _articles($region = FALSE, $search = '', $limit = 20, $pager = TRUE){
  $url = 'http://all.culture.ru/api/1.0/articles';
  $decode = _decode($url, $region, $search, $limit);

  $output = '';

  if ($decode->total == 0) {
    $output = '<div class="not-found">Статей с заданными параметрами не найдено.</div>';
  }
  else{
    foreach ($decode->articles as $key => $article) {
      $region_url = _return_current_region_url($article->localeIds);
      $article_url = '/articles/' . $article->_id;
      $created = date('d.m.Y', $article->createDate);
      $text = '';
      foreach ($article->content as $key => $content) {
        if ($content->type == 'text') {
          $text = $content->text;;
        }
      }
      $output .= theme('articles', 
        array(
          'title' => l($article->name, $article_url),
          'image' => _image($article->image->name, 'image'),
          'text' => truncate_utf8(strip_tags($text), 500, TRUE, TRUE),
          'created' => $created,
        )
      );
    }

  $output .= ($pager) ? _pager($decode->total) : '';

  }
  return $output;
}

//выводит одно событие
function _article($aid){
  $output = '';
  //декодируем урл статьи
  $url = 'http://all.culture.ru/api/1.0/articles?ids='.$aid;
  $decode = _decode($url, FALSE, '', 0); //false - значит, что мы не применяем фильтр региона

  if(!isset($decode->articles[0])){
    return drupal_not_found();  
  }
  else{
    //вытягиваем переменные события
    $article = $decode->articles[0];                            //определяем массив с самим событием
    $image = $article->image;                                 //получаем картинку события
    $name = $article->name;                                   //получаем название события
    $organization = ($article->organization) ? $article->organization->name : NULL;  //если есть место проведения, то место
    $region_url = _return_current_region_url($article->localeIds);
    $category = $article->category->name;                     //название категории события
    $created = date('d.m.Y', $article->createDate);
    $text = '';
    $images_array = array();
    foreach ($article->content as $key => $content) {
      if ($content->type == 'text') {
        $text = $content->text;
      }
      else if ($content->type == 'images') {
        $images_array = $content->images;
      }
    }

    //это для вывода в .tpl файл
    $output = theme('article', 
      array(
        'title' => $name,
        'image' => _image($image->name),
        'created' => $created,
        'text' => $text,
        'category' => l($category, 'category/'.$article->category->_id),
        'tags' => _tags($article->tags),
        'organization' => l($article->organization->name, 'organizations/'.$article->organization->_id),
        'gallery' => _gallery($images_array),
      )
    );

    //задаём тайтл для события
    $title[] = $name;
    $title[] = $category;
    if($organization){ $title[] = $organization; }
    drupal_set_title(implode(' - ', $title));

    //задаём мета-тэги
    $keywords_array = array();
    $keywords_array[] = $category;
    $keywords_array[] = strip_tags(_tags($article->tags));   
    _set_seo_tags($keywords_array, '');

  }
  return $output;

}