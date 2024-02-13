package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Range;
import xxl.core.Spreadsheet;
import xxl.core.exception.PasteException;
import xxl.core.exception.UnrecognizedEntryException;


/**
 * Paste command.
 */
class DoPaste extends Command<Spreadsheet> {

  DoPaste(Spreadsheet receiver) {
    super(Label.PASTE, receiver);
    addStringField("address", Message.address());
  }
  
  @Override
  protected final void execute() throws CommandException {
    String address = stringField("address");
    try {
      Range range = _receiver.createRange(address);
      _receiver.getBuffer().paste(range);
    }
    catch (UnrecognizedEntryException | PasteException e) {
      throw new InvalidCellRangeException(address);
    }
  }
}
