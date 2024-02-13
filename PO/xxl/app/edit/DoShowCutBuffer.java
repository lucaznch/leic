package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import xxl.core.Spreadsheet;
// FIXME import classes

/**
 * Show cut buffer command.
 */
class DoShowCutBuffer extends Command<Spreadsheet> {

  DoShowCutBuffer(Spreadsheet receiver) {
    super(Label.SHOW_CUT_BUFFER, receiver);
  }
  
  @Override
  protected final void execute() {
    String[] cellsToView = _receiver.viewBuffer();
    for (String s: cellsToView) {
      _display.addLine(s);
    }
    _display.display();
  }
}
