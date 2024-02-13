package xxl.core;

import java.util.*;


public class User {
    private final String _name;
    private List<Spreadsheet> _spreadsheets; /** one user has a set of spreadsheets  */
    

    public User(String name) {
        _name = name;
        _spreadsheets = new ArrayList<Spreadsheet>();
    }


    /** @return - name of user */
    public String getName() {
        return _name;
    }


    /** adds a spreadsheet to this user */
    public void associateSpreadhsheet(Spreadsheet s) {
        if (!_spreadsheets.contains(s)) {
            _spreadsheets.add(s);
        }
    }

    
    @Override
    public int hashCode() {
        return _name.hashCode();
    }


    public boolean equals(User user) {
        return _name.equals(user.getName());
    }
}