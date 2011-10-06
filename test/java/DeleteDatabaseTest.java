import com.scalien.scaliendb.*;

public class DeleteDatabaseTest {
	
	public static void deleteDatabaseTest(Client client) throws SDBPException {
        for (Database database: client.getDatabases ()) {
            if (database.getName ().equals ("iglue")) {
                database.deleteDatabase ();
				System.out.println("Database \"" + database.getName() + "\" deleted");
                break;
            }
		}
	}
	
	public static void main(String[] args) {
        try {
            String[] controllers = {"192.168.137.100:7080", "192.168.137.101:7080", "192.168.137.102:7080"};
			ConfigLoader configLoader = new ConfigLoader(controllers);
			controllers = configLoader.getControllers();

            Client client = new Client(controllers);
            // client.setTrace(true);

			deleteDatabaseTest(client);
            
        } catch (Exception e) {
            System.err.println("Exception: " + e.toString());
        }
		
	}
}
