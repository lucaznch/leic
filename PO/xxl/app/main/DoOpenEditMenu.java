package xxl.app.main;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Calculator;

/**
 * Open menu.
 */
class DoOpenEditMenu extends Command<Calculator> {

  DoOpenEditMenu(Calculator receiver) {
    super(Label.MENU_CALC, receiver, r -> r.getSpreadsheet() != null);
  }
  
  @Override
  protected final void execute() throws CommandException {
    (new xxl.app.edit.Menu(_receiver.getSpreadsheet())).open();
  }
}
