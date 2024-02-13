package xxl.app.main;

/**
 * Messages.
 */
interface Message {

  /**
   * @return string with "file not found" message.
   */
  static String fileNotFound() {
    return "O ficheiro não existe.";
  }
  
  /**
   * @param filename the missing file
   * @return string with "file not found" message (more elaborate).
   */
  static String fileNotFound(String filename) {
    return "O ficheiro '" + filename + "' não existe.";
  }

  /**
   * @param cause the original problem
   * @return string with problem description.
   */
  static String problemOpeningFile(Exception cause) {
    return "Problema ao abrir ficheiro: " + cause.getMessage();
  }

  /** @return string with prompt for filename to open. */
  static String openFile() {
    return "Ficheiro a abrir: ";
  }
  
  /** @return string with a warning and a question. */
  static String newSaveAs() {
    return "Ficheiro sem nome. " + saveAs();
  }

  /** @return string asking for a filename. */
  static String saveAs() {
    return "Guardar ficheiro como: ";
  }

  /** @return string confirming that user wants to save. */
  static String saveBeforeExit() {
    return "Guardar antes de fechar? ";
  }
  
  /** @return string with prompt for number of lines. */
  static String lines() {
    return "Especifique o número de linhas da folha: ";
  }

  /** @return string with prompt for number of columns. */
  static String columns() {
    return "Especifique o número de colunas da folha: ";
  }
}
