  $('.likebutton').click(function () {    
    $.ajax({
        url: "{% url 'ajax_question_like' %}",
        type: "POST",
        data: {'id': $(this).attr('questionid'), 'value' : '1', 'csrfmiddlewaretoken': '{{ csrf_token }}'},
        success: function(response) {
          // $(response.qstyleid).text(response.qrating);
          // $(response.qstyleid).removeClass().addClass(response.qstyle);
          // $(response.ustyleid).text(response.urating);
          // $(response.ustyleid).removeClass().addClass(response.ustyle);
          // $(response.likebuttonid).removeClass().addClass(response.likebuttonstyle);
          // $(response.dislikebuttonid).removeClass().addClass(response.dislikebuttonstyle);
            
            }
        });
  });