package xxl.app;

import pt.tecnico.uilib.Dialog;
import xxl.core.exception.ImportFileException;

import java.io.IOException;

/**
 * Class that represents the spreadsheet's textual interface.
 */
public class App {

  public static void main(String[] args) {
    try (var ui = Dialog.UI) {
      var receiver = new xxl.core.Calculator();
      String datafile = System.getProperty("import");
      if (datafile != null) {
        try {
          receiver.importFile(datafile);
        } catch (ImportFileException e) {
          // no behavior described: just present the problem
          e.printStackTrace();
        }
      }
      
      (new xxl.app.main.Menu(receiver)).open();
    }
  }
}
