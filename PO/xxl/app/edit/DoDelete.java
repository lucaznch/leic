package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Range;
import xxl.core.Spreadsheet;
import xxl.core.exception.UnrecognizedEntryException;

/**
 * Delete command.
 */
class DoDelete extends Command<Spreadsheet> {

  DoDelete(Spreadsheet receiver) {
    super(Label.DELETE, receiver);
    addStringField("address", Message.address());
  }
  
  @Override
  protected final void execute() throws CommandException {
    String address = stringField("address");
    try {
      Range range = _receiver.createRange(address);
      _receiver.getBuffer().delete(range);
    }
    catch (UnrecognizedEntryException e) {
      throw new InvalidCellRangeException(address);
    }
  }
}
