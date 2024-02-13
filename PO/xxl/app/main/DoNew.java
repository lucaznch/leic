package xxl.app.main;

import pt.tecnico.uilib.forms.*;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Calculator;

/**
 * Open a new file.
 */
class DoNew extends Command<Calculator> {

  DoNew(Calculator receiver) {
    super(Label.NEW, receiver);
    addIntegerField("lines", Message.lines());
    addIntegerField("columns", Message.columns());
  }
  
  @Override
  protected final void execute() throws CommandException {
    if (!_receiver.isSpreadsheetSaved()) { /** if spreadsheet is not saved, ask user about saving it */
      Form form = new Form();
      form.addStringField("answer", Message.saveBeforeExit());
      String answer = form.stringField("answer");

      if (answer.equalsIgnoreCase("sim") || answer.equalsIgnoreCase("yes") || answer.equalsIgnoreCase("s") || answer.equalsIgnoreCase("y")) {
        DoSave saver = new DoSave(_receiver);
        saver.execute();
      }
    }

    Integer numLines = integerField("lines");
    Integer numColumns = integerField("columns");

    _receiver.createNewSpreadsheet(numLines, numColumns);
  }
}