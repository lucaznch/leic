package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Range;
import xxl.core.Spreadsheet;
import xxl.core.exception.UnrecognizedEntryException;
// FIXME import classes

/**
 * Class for searching functions.
 */
class DoShow extends Command<Spreadsheet> {

  DoShow(Spreadsheet receiver) {
    super(Label.SHOW, receiver);
    addStringField("address", Message.address());
  }
  
  @Override
  protected final void execute() throws CommandException {
    String address = stringField("address");

    try {
      Range range = _receiver.createRange(address);
      String[] cellsToView = _receiver.view(range);
      for (String s: cellsToView) {
        _display.addLine(s);
      }
      _display.display();
    }
    catch (UnrecognizedEntryException e) {
      throw new InvalidCellRangeException(address); /** throw command exception !!! */
    }
  }
}