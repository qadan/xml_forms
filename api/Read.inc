<?php

/**
 * @file
 * Class to implement the Read action.
 */

use Drupal\objective_forms\FormElement;

module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');

/**
 * Read implementation of the ActionInterface class.
 *
 * @codingStandardsIgnoreStart
 * XXX: Poorly implemented sniff indicates class should begin with
 * "XMLFormApi.info" instead of just "XMLFormApi"... ideally, this
 * should move somewhere under the Drupal\xml_schema_api namespace at
 * somepoint... because of this, we've left it with only the original
 * prefix, to avoid having to carry through the change.
 */
class Read implements ActionInterface {
// @codingStandardsIgnoreEnd

  /**
   * Path to the parent element, where the DOMNode will be created.
   *
   * @var Path
   */
  protected $path;

  /**
   * Construct Read ActionInterface.
   *
   * @param array $params
   *   An array containing two elements:
   *   'path' - the XPath to be used in this instance of Read.
   *   'context' - an instance of ContextType (__DEFAULT/DOCUMENT/PARENT/SELF).
   */
  public function __construct(array &$params) {
    $this->path = new Path($params['path'], new Context(new ContextType($params['context'])));
  }

  /**
   * Retrieves an array of values that can be passed on to a Drupal form.
   *
   * @return array
   *   The array of return values.
   */
  public function toDrupalForm() {
    return [
      'path' => $this->path->path,
      'context' => (string) $this->path->context,
    ];
  }

  /**
   * Determines whether or not we should execute an action on a form element.
   *
   * @param XMLDocument $document
   *   The document we want to check.
   * @param \Drupal\objective_forms\FormElement $element
   *   The form element inside the document we want to check.
   * @param mixed $value
   *   The value of the element we want to check.
   *
   * @return bool
   *   Always TRUE.
   */
  public function shouldExecute(XMLDocument $document, FormElement $element, $value = NULL) {
    // It's always safe to read!
    return TRUE;
  }

  /**
   * Executes the action on the form element.
   *
   * @param XMLDocument $document
   *   The document we want to check.
   * @param \Drupal\objective_forms\FormElement $element
   *   The form element inside the document we want to check.
   * @param mixed $value
   *   The value of the element we want to check.
   *
   * @return DOMNodeList
   *   A list of nodes to return.
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    try {
      $results = $this->path->query($document, $element);
    }
    catch (XMLFormsContextNotFoundException $e) {
      // Ignore context not found, in this case the context node
      // should be created during the submit process.
      $results = new DOMNodeList();
    }
    return $results;
  }

}
