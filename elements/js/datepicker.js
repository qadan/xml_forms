/**
 * @file
 * Date picker controls.
 */

(function ($, Drupal) {
    Drupal.behaviors.xmlFormElementDatepicker = {
        attach: function(context, settings) {
        $( "input.datepicker" ).datepicker({
                changeMonth: true,
                changeYear: true,
                dateFormat: 'yy-mm-dd'
            });
        }
    };
})(jQuery, Drupal);
