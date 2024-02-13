package xxl.app.exception;

import pt.tecnico.uilib.menus.CommandException;

/**
 * Thrown when an unknown function is used.
 */
public class UnknownFunctionException extends CommandException {
  private final String _functionName;
  
  /** @param functionName  */
  public UnknownFunctionException(String functionName) {
    super("A função '" + functionName + "' é desconhecida.");
    _functionName = functionName;
  }

  public final String getFunctionName() {
    return _functionName;
  }
}
