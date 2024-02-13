package xxl.app.exception;

import pt.tecnico.uilib.menus.CommandException;

/** Exception for reporting general problems opening and processing files. */
public class FileOpenFailedException extends CommandException {
  public FileOpenFailedException(Exception e) {
    super(Message.problemOpeningFile(e), e);
  }
}
