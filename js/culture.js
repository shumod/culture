(function($) {
  $(document).ready(function() {

    $('#edit-region').change(function() { 
      var region = $(this).find('option:selected').val();
      var destination = window.location.pathname;
      url = 'set-region/' + region + '?destination=' + destination;
      $(location).attr('href',url);
      
    });

    $('.gallery').slick({
      dots: true,
      infinite: true,
      speed: 300,
      slidesToShow: 3,
      centerMode: true,
      lazyLoad: 'ondemand',
      //variableWidth: true
    });

    $('.event-seances').readmore({
      speed: 75,
      maxHeight: 145,
      moreLink: '<a href="#">Подробнее</a>',
      lessLink: '<a href="#">Скрыть</a>'
    });

    $('.event-persons').readmore({
      speed: 75,
      maxHeight: 145,
      moreLink: '<a href="#">Подробнее</a>',
      lessLink: '<a href="#">Скрыть</a>'
    });

  });
})(jQuery);