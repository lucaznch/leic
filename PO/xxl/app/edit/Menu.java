package xxl.app.edit;

import xxl.core.Spreadsheet;

/**
 * Menu builder for editing operations.
 */
public class Menu extends pt.tecnico.uilib.menus.Menu {

  public Menu(Spreadsheet receiver) {
    super(Label.TITLE, //
          new DoShow(receiver), //
          new DoInsert(receiver), //
          new DoCopy(receiver), //
          new DoDelete(receiver), //
          new DoCut(receiver), //
          new DoPaste(receiver), //
          new DoShowCutBuffer(receiver) //
          );
  }
}
