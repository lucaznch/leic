package xxl.core;

import java.io.*;
import java.util.*;
import xxl.core.exception.ImportFileException;
import xxl.core.exception.MissingFileAssociationException;
import xxl.core.exception.UnavailableFileException;
import xxl.core.exception.UnrecognizedEntryException;



public class Calculator {
    private Spreadsheet _spreadsheet;
    private String _filename;
    private User _user;
    private Map<String, User> _allUsers;



    public Calculator() {
        _user = new User("root");
        _allUsers = new HashMap<>();
        _allUsers.put("root", _user);
    }


    /**
    * gets the current spreadsheet
    *
    * @return - current spreadsheet of the application (it can be null)
    */
    public final Spreadsheet getSpreadsheet() {
        return _spreadsheet;
    }


    /**
    * gets the current user
    *
    * @return - current user of the application (it can be null)
    */
    public final User getUser() {
        return _user;
    }


    /** 
    * adds a user
    * @param user - user to be added
    */
    public void addUser(User user) {
        _allUsers.put(user.getName(), user);
    }


    /** 
    * sets the active user
    * @param user - user to be defined
    */
    public void setUser(User user) {
        _user = user;
    }


    /**
    * creates a new spreadsheet in the application with a fixed number of rows and columns
    *
    * @param rows - number of rows
    * @param columns - number of columns
    */
    public void createNewSpreadsheet(int rows, int columns) {
        _spreadsheet = new Spreadsheet(rows, columns);
        _user.associateSpreadhsheet(_spreadsheet);
    }


    /** @return - true if application has a file associated */
    public boolean hasAssociate() {
        return _filename != null;
    }


    /**
    * reads a text input file and creates domain entities.
    *
    * @param filename - name of the text input file
    * @throws - ImportFileException
    */
    public void importFile(String filename) throws ImportFileException {
        Parser parser = new Parser();
        try {
            _spreadsheet = parser.parseFile(filename);
            _spreadsheet.setParser(parser);
        } 
        catch (IOException | UnrecognizedEntryException e) {
            throw new ImportFileException(filename, e);
        }
    }


    /** @return - true if spreadsheet is saved or if there is no spreadsheet yet in the application */
    public boolean isSpreadsheetSaved() {
        if (_spreadsheet == null)
            return true;
        else
            return _spreadsheet.getSavedStatus();
    }


    /**
    * Saves the serialized application's state into the file associated to the current network.
    *
    * @throws FileNotFoundException if for some reason the file cannot be created or opened. 
    * @throws MissingFileAssociationException if the current network does not have a file.
    * @throws IOException if there is some error while serializing the state of the network to disk.
    */
    public void save() throws FileNotFoundException, MissingFileAssociationException, IOException {
        if (_filename == null) {
            throw new MissingFileAssociationException();
        }
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(_filename))) {
            _spreadsheet.setSavedStatus(true);
            oos.writeObject(_spreadsheet); /* serialize spreadsheet object */
        } catch (Exception e) {
            _spreadsheet.setSavedStatus(false);
            e.printStackTrace();
        }
    }


    /**
    * Saves the serialized application's state into the specified file. The current network is
    * associated to this file.
    *
    * @param filename the name of the file.
    * @throws FileNotFoundException if for some reason the file cannot be created or opened.
    * @throws MissingFileAssociationException if the current network does not have a file.
    * @throws IOException if there is some error while serializing the state of the network to disk.
    */
    public void saveAs(String filename) throws FileNotFoundException, MissingFileAssociationException, IOException {
        _filename = filename;
        save();
    }


    /**
    * @param filename - name of the file containing the serialized application's state to load.
    * @throws - UnavailableFileException if the specified file does not exist or there is an error while processing this file.
    */
    public void load(String filename) throws UnavailableFileException, FileNotFoundException {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(filename))) {
            _spreadsheet = (Spreadsheet)ois.readObject();
            _spreadsheet.setParser(new Parser(_spreadsheet));
            /** clear buffer ? */
        }
        catch (IOException | ClassNotFoundException e) {
            throw new UnavailableFileException(filename);
        }
    }

}