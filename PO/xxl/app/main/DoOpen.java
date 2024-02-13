package xxl.app.main;

import java.io.*;
import pt.tecnico.uilib.forms.*;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.FileOpenFailedException;
import xxl.core.Calculator;
import xxl.core.exception.UnavailableFileException;

/**
 * Open existing file.
 */
class DoOpen extends Command<Calculator> {

  DoOpen(Calculator receiver) {
    super(Label.OPEN, receiver);
    addStringField("filename", Message.openFile());
  }
  
  @Override
  protected final void execute() throws CommandException {
    if (!_receiver.isSpreadsheetSaved()) {
      Form form = new Form();
      form.addStringField("answer", Message.saveBeforeExit());
      String answer = form.stringField("answer");

      if (answer.equalsIgnoreCase("sim") || answer.equalsIgnoreCase("yes") || answer.equalsIgnoreCase("s") || answer.equalsIgnoreCase("y")) {
        DoSave saver = new DoSave(_receiver);
        saver.execute();
      }
    }

    try {
      _receiver.load(stringField("filename"));
    }
    catch (UnavailableFileException | FileNotFoundException e) {
      throw new FileOpenFailedException(e);
    }
  }
}
