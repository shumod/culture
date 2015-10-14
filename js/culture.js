(function($) {
  $(document).ready(function() {

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

  });
})(jQuery);