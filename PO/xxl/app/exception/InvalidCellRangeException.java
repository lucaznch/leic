package xxl.app.exception;

import pt.tecnico.uilib.menus.CommandException;

/**
 * Thrown when an invalid cell range is used.
 */
public class InvalidCellRangeException extends CommandException {
  private final String _range;
  
  /** @param range  */
  public InvalidCellRangeException(String range) {
    super("A gama '" + range + "' é inválida.");
    _range = range;
  }

  public final String getInvalidRange() {
    return _range;
  }
}
