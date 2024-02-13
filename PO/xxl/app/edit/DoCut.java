package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Range;
import xxl.core.Spreadsheet;
import xxl.core.exception.UnrecognizedEntryException;

/**
 * Cut command.
 */
class DoCut extends Command<Spreadsheet> {

  DoCut(Spreadsheet receiver) {
    super(Label.CUT, receiver);
    addStringField("address", Message.address());
  }
  
  @Override
  protected final void execute() throws CommandException {
    String address = stringField("address");
    try {
      Range range = _receiver.createRange(address);
      _receiver.getBuffer().cut(range);
    }
    catch (UnrecognizedEntryException e) {
      throw new InvalidCellRangeException(address);
    }
  }
}
