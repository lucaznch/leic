package xxl.app.search;

import pt.tecnico.uilib.menus.Command;
import xxl.core.Spreadsheet;
// FIXME import classes

/**
 * Command for searching function names.
 */
class DoShowFunctions extends Command<Spreadsheet> {

  DoShowFunctions(Spreadsheet receiver) {
    super(Label.SEARCH_FUNCTIONS, receiver);
    addStringField("function", Message.searchFunction());
  }

  @Override
  protected final void execute() {
    String functionName = stringField("function");
    String[] cellsToView =_receiver.searchFunction(functionName);

    for (String s: cellsToView) {
      _display.addLine(s);
    }
    _display.display();
  }
}
