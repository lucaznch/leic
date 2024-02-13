package xxl.app.edit;

/**
 * Messages.
 */
interface Message {
  /** @return string with prompt for range. */
  static String address() {
    return "Especifique a gama (startline;startcol:endline;endcol): ";
  }
  
  /** @return string with prompt for content. */
  static String contents() {
    return "Insira o conteúdo da célula: ";
  }
}
