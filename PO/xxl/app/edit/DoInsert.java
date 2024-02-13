package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Range;
import xxl.core.Spreadsheet;
import xxl.core.exception.UnrecognizedEntryException;
// FIXME import classes

/**
 * Class for inserting data.
 */
class DoInsert extends Command<Spreadsheet> {

  DoInsert(Spreadsheet receiver) {
    super(Label.INSERT, receiver);
    addStringField("address", Message.address());
    addStringField("contents", Message.contents());
  }
  
  @Override
  protected final void execute() throws CommandException {
    String address = stringField("address");
    String contentSpecification = stringField("contents");

    try {
      Range range = _receiver.createRange(address);
      _receiver.insertContent(range, contentSpecification);
    }
    catch (UnrecognizedEntryException e) {
      throw new InvalidCellRangeException(address);
    }
  }
}
