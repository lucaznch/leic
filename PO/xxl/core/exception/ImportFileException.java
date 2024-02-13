package xxl.core.exception;

/**
 * Class for representing an import problem.
 */
public class ImportFileException extends Exception {
  private static final String ERROR_MESSAGE = "Erro a processar ficheiro de import: ";

  /**
   * @param filename name of the import file
   */
  public ImportFileException(String filename) {
    super(ERROR_MESSAGE + filename);
  }
  
  /**
   * @param filename name of the import file
   * @param cause exception that triggered this one
   */
  public ImportFileException(String filename, Exception cause) {
    super(ERROR_MESSAGE + filename, cause);
  }
  
}
