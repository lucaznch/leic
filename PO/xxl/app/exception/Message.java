package xxl.app.exception;

/** Messages for error reporting. */
interface Message {
  static String problemOpeningFile(Exception cause) {
    return "Problema ao abrir ficheiro: " + cause.getMessage();
  }
}
