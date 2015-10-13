(function($) {
  $(document).ready(function() {

    $('#edit-region').change(function() { 
      var cookie = $(this).find('option:selected').val();
      $.cookie('region', cookie, { expires: 365 });

      $('#region-cookies').hide().html('Регион сменён').fadeIn(500);
      location.reload();
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

  });
})(jQuery);