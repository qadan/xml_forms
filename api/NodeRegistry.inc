<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'XMLDocument');
module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 * Tracks the nodes FormElements repersent.
 */
class NodeRegistry { // implements IteratorAggregate {

  /**
   * Only valid when we serialize this object. It stores the paths to registered nodes 
   * so they can be properly reinitialized on unserialize.
   * 
   * @var array
   */

  protected $paths;
  /**
   * A map of FormElements to the DOMNodes the repersent. These nodes are used for populating the form elements
   * as well as a reference to what DOMNodes need to be modified/removed on submit.
   * 
   * @var array
   */
  protected $registered;
  /**
   * The document that the registered nodes come from.
   * 
   * @var document
   */
  public $document;

  /**
   * Create the node registry.
   * 
   * @param Document $document 
   */
  public function __construct(XMLDocument $document) {
    $this->document = $document;
    $this->registered = array();
  }

  /**
   * Determine what member variables to serialize.
   * 
   * @return array
   */
  public function __sleep() {
    $this->paths = array();
    foreach ($this->registered as $key => $node) {
      $this->paths[$key] = get_dom_node_xpath($node);
    }
    return array('paths', 'document');
  }

  /**
   * Reinitialize this object after it has be unserialized.
   */
  public function wakeup() {
    $this->registered = array();
    foreach ($this->paths as $key => $path) {
      $result = $this->document->query($path);
      $this->registered[$key] = $result->item(0);
    }
  }

  /**
   * Register a relationship between a FormElement and a DOMNode.
   * 
   * @param hash $hash
   * @param DOMNode $node 
   */
  public function register($hash, DOMNode $node) {
    $this->registered[$hash] = $node;
  }

  /**
   * Is the given DOMNode registered?
   * 
   * @param DOMNode $node 
   * @return boolean
   *   TRUE if the given DOMNode is already registered 
   */
  public function isNodeRegistered(DOMNode $node) {
    $hash = $this->getElementHash($node);
    if (isset($hash)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Is the given FormElement registered?
   * 
   * @param hash $hash
   * @return boolean
   *   TRUE if the element is registered FALSE otherwise.
   */
  public function isElementRegistered($hash) {
    return $this->getNode($hash) != NULL;
  }

  /**
   * Find the hash for the element that corresponds to the given DOMNode.
   * 
   * @param DOMNode $node
   * @return hash
   */
  public function getElementHash(DOMNode $node) {
    $result = array_filter($this->registered, array($node, 'isSameNode'));
    if (count($result) == 1) {
      $keys = array_keys($result);
      return array_shift($keys);
    }
    return NULL;
  }

  /**
   * Get the registered DOMNode associated with the given FormElement.
   * 
   * @param hash $hash
   * @return DOMNode
   */
  public function getNode($hash) {
    if (isset($this->registered[$hash])) {
      return $this->registered[$hash];
    }
    return NULL;
  }

}