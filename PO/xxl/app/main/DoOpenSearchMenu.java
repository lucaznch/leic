package xxl.app.main;

import pt.tecnico.uilib.forms.Form;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Calculator;

/**
 * Open menu.
 */
class DoOpenSearchMenu extends Command<Calculator> {
  DoOpenSearchMenu(Calculator receiver) {
    super(Label.MENU_SEARCH, receiver, r -> r.getSpreadsheet() != null);
  }
  
  @Override
  protected final void execute() throws CommandException {
    (new xxl.app.search.Menu(_receiver.getSpreadsheet())).open();
  }
}
