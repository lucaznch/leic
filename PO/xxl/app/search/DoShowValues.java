package xxl.app.search;

import pt.tecnico.uilib.menus.Command;
import xxl.core.Spreadsheet;
// FIXME import classes

/**
 * Command for searching content values.
 */
class DoShowValues extends Command<Spreadsheet> {

  DoShowValues(Spreadsheet receiver) {
    super(Label.SEARCH_VALUES, receiver);
    addStringField("value", Message.searchValue());
  }
  
  @Override
  protected final void execute() {
    String valueSpecification = stringField("value");
    String[] cellsToView;

    if (valueSpecification.charAt(0) == '\'') {
      cellsToView = _receiver.searchStringValue(valueSpecification.substring(1));
    }
    else {
      cellsToView = _receiver.searchIntValue(Integer.parseInt(valueSpecification));
    }

    for (String s: cellsToView) {
      _display.addLine(s);
    }
    _display.display();
  }
}
