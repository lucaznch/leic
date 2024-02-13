package xxl.app.main;

import java.io.*;
import pt.tecnico.uilib.forms.Form;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.FileOpenFailedException;
import xxl.core.Calculator;
import xxl.core.exception.MissingFileAssociationException;


/**
 * Save to file under current name (if unnamed, query for name).
 */
class DoSave extends Command<Calculator> {

  DoSave(Calculator receiver) {
    super(Label.SAVE, receiver, xxl -> xxl.getSpreadsheet() != null);
  }
  
  @Override
  protected final void execute() throws CommandException {
    try {
      if (!_receiver.hasAssociate()) {
        Form form = new Form();
        form.addStringField("filename", Message.newSaveAs());
        // form.parse();

        _receiver.saveAs(form.stringField("filename"));
        
        return;
      }
      _receiver.save();
    }
    catch (MissingFileAssociationException | IOException e) {
      throw new FileOpenFailedException(e);
    }
  }
}
