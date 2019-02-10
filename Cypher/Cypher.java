import java.io.*;
import java.util.regex.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

public class Cypher {
	private Map<Character,String[]> alphabet = new HashMap<Character,String[]>();
	private final BufferedReader language,input;
	private String regex = "^[a-zA-Z]:.*,?[^,]$"; //leet by default
	
	public Cypher(final String inputLanguage , final String inputFile) throws FileNotFoundException , IOException {
		language = new BufferedReader(new FileReader(inputLanguage));
		input    = new BufferedReader(new FileReader(inputFile));
	}
	
	public void setRegex(String regex){
		this.regex = regex;
	}
	
	public boolean checkLine(String line){
		Pattern pattern = Pattern.compile(regex);
		Matcher match   = pattern.matcher(line);
		return match.lookingAt();
	}
	
	public void buildAlphabet() throws IOException{
		String line = null;
		
		while ( (line = language.readLine()) != null){
			if (checkLine(line) == true) {
				System.out.println(line);
				alphabet.put((Character.toLowerCase(line.charAt(0))),(line.substring(line.substring(1).indexOf(" ") + 1)).split(","));
				alphabet.put((Character.toUpperCase(line.charAt(0))),(line.substring(line.substring(1).indexOf(" ") + 1)).split(","));

			}
		}
	}
	
	public String convertText(String input){
		String result = new String("");
		Random rand = new Random();
		for (int i = 0 ; i < input.length() ; i++) 
			if ( alphabet.containsKey(input.charAt(i)) ) 
				result += alphabet.get(input.charAt(i))[rand.nextInt(alphabet.get(input.charAt(i)).length)];
			else
				result += input.charAt(i);
		return result;
	}
	
	public void convertFile() throws IOException{
		String line = null;
		System.out.println("=====>CONVERTED TEXT:<=====");
		while( (line = input.readLine() ) != null){
			System.out.println(convertText(line));
		}
		System.out.println("^^^CONVERTED TEXT^^^");
	}
	
	static void usage(){
		System.out.println("The program was not executed correctly\nUsage: java Cypher alphabetFile inputFile\n\nInside the alphabet file ," +
				" any language fitting the following example can be utilized:  [letter]:changeTo,changeTo2...changeToN");
		System.exit(-1);
	}
	
	
	public static void main(String[] args){
		if (args.length < 2) usage();
		try {
			Cypher cypher = new Cypher(args[0],args[1]);
			cypher.buildAlphabet();
			cypher.convertFile();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}
}