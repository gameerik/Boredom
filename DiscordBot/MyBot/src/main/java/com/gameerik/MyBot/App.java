package com.gameerik.MyBot;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Member;

import net.dv8tion.jda.core.JDA;
import net.dv8tion.jda.core.JDABuilder;
import net.dv8tion.jda.core.MessageBuilder;
import net.dv8tion.jda.core.entities.ChannelType;
import net.dv8tion.jda.core.entities.IMentionable;
import net.dv8tion.jda.core.entities.Invite.Guild;
import net.dv8tion.jda.core.entities.Message;
import net.dv8tion.jda.core.entities.MessageChannel;
import net.dv8tion.jda.core.entities.TextChannel;
import net.dv8tion.jda.core.entities.User;
import net.dv8tion.jda.core.entities.VoiceChannel;
import net.dv8tion.jda.core.events.message.MessageReceivedEvent;
import net.dv8tion.jda.core.events.message.guild.GuildMessageReceivedEvent;
import net.dv8tion.jda.core.hooks.ListenerAdapter;
import net.dv8tion.jda.core.managers.AudioManager;
import net.dv8tion.jda.core.managers.GuildManager;

import java.util.ArrayList;
import java.util.Random;


public class App extends ListenerAdapter {
	private static Random rand = new Random();
	private static ArrayList<String> burn = new ArrayList<String>();
	private static ArrayList<String> payback = new ArrayList<String>();
	private static String[] gifs;
	private static String fileName = "Burn.txt";
	private static String fileName2 = "Payback.txt";
	private static String target = "frostweaver";
	
    public static void main( String[] args ) throws Exception {
    	buildGifs();
    	JDA jda = new JDABuilder("your personal bot token goes here").addEventListener(new App()).build();
    	jda.awaitReady();
    	System.out.println("Building finished");
    }
    
    public App() throws IOException {
    	//buildBurn();
    	BufferedReader br = new BufferedReader(new FileReader(fileName));
    	String line = null;
    	while ( (line = br.readLine()) != null ){
    		burn.add(line);
    	}
    	BufferedReader br2 = new BufferedReader(new FileReader(fileName2));
    	line = null;
    	while ( (line = br2.readLine()) != null ){
    		payback.add(line);
    	}
    }
   
    
   
    
    //text---------------------------------------------------------------
    @Override
    public void onMessageReceived(MessageReceivedEvent event) {
    	JDA jda = event.getJDA();
    	long responseNumber = event.getResponseNumber();
    	
    	
    	User author = event.getAuthor();
    	
    	Message message = event.getMessage();
    	MessageChannel channel = event.getChannel();
    	

    	
    	String msg = message.getContentDisplay();
    	
    	boolean bot = author.isBot();
    	
    	if (event.isFromType(ChannelType.TEXT)) {
    		net.dv8tion.jda.core.entities.Guild guild = event.getGuild();
    		TextChannel textChannel = event.getTextChannel();
    		net.dv8tion.jda.core.entities.Member member = event.getMember();
    		
    		String name;
    		
    		if (message.isWebhookMessage()) 
    			name = author.getName();
    		else 
    			name = member.getEffectiveName();
    		
            System.out.printf("(%s)[%s]<%s>: %s\n", guild.getName(), textChannel.getName(), name, msg);
         // commands --------------------------------------------------------------------------------------------------------------------------------------------------------------   
            if (msg.equals("!edi"))
             	channel.sendMessage((burn.get(rand.nextInt(burn.size())))).queue();

            
            
            if(name.equals(target) && !msg.equals("!edi") && !bot && !msg.contains("!addBurn") && !msg.contains("!addPayback") ) 	
            	if (!msg.contains("http")) channel.sendMessage(msg + payback.get(rand.nextInt(payback.size()))).addFile(new File(gifs[rand.nextInt(gifs.length)])).complete();
            	else channel.sendMessage(payback.get(rand.nextInt(payback.size()))).addFile(new File(gifs[rand.nextInt(gifs.length)])).complete();
            
            if(!name.equals(target) && msg.contains("!addBurn") && !bot && (msg.length() > 8)) {
            	try {
            		addBurn(msg.substring(8),jda);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
            	channel.sendMessage(msg.substring(8) + " has been added to the burn list").queue();
            	
            }
            
            if (msg.equals("!help")) 
            	channel.sendMessage("The current commands available are:\n!edi\n	^use this command to burn @frostweaver\n"
            						+ "!addBurn\n	^use this command to add to the \"!edi\" burn List (if it already contains the burn it won't be added again) [optional parameter \"<edi>\"(without quotes)] to place @frostweaver in that position(can be placed only once))\n"
            						+ "!addPayback\n	^use this command to add to the \"payback\" list\n"
            						+ "!swarm\n	   ^use this command to send 5 \"!edi\" in a row").queue();
            
            if(!name.equals(target) && msg.contains("!addPayback") && !bot && (msg.length() > 11)) {
            	try {
					addPayback(msg.substring(11));
				} catch (IOException e) {
					e.printStackTrace();
				}
            	
            	channel.sendMessage(msg.substring(11) + " has been added to the payback list").queue();
            }
            
            if (name.equals("gameerik") && msg.contains("clyde")) 
            	channel.sendMessage("I wish my creator " + jda.getUsersByName("gameerik", true).get(0).getAsMention() + " never built me" ).queue();
    	}
    		if (msg.equals("!swarm")) {
    			for (int i = 0 ; i < 5;i++)
    				channel.sendMessage(swarm()[i]).queue();
    		}
            //end commands --------------------------------------------------------------------------------------------------------------------------------------------------------------   

    }
    
    
    
    static void addBurn(String message , JDA jda) throws IOException {
    	for (String i : burn) 
    		if (i.equalsIgnoreCase(message)) return;
    	
    	
    	BufferedWriter bw = new BufferedWriter(new FileWriter(fileName,true));
    	if (!message.contains("<edi>")) {
    		bw.append(jda.getUsersByName(target, true).get(0).getAsMention() + " " + message + '\n');
    		burn.add(jda.getUsersByName(target, true).get(0).getAsMention() + " " + message);
    	}
    	else {
    		bw.append(message.substring(0,message.indexOf("<edi>")) +jda.getUsersByName(target, true).get(0).getAsMention() + message.substring(message.indexOf("<edi>") + "<edi>".length()) + '\n');
    		burn.add(message.substring(0,message.indexOf("<edi>")) +jda.getUsersByName(target, true).get(0).getAsMention() + message.substring(message.indexOf("<edi>") + "<edi>".length()));
    	}
       	bw.close();
    }
    static void addPayback(String message) throws IOException {
    	for (String i : payback) 
    		if (i.equalsIgnoreCase(message)) return;
    	payback.add(" " +message);
    	BufferedWriter bw = new BufferedWriter(new FileWriter(fileName2,true));
    	bw.append(message + '\n');
    	bw.close();
    }
    
     /* static void buildBurn() {
		burn.add(" is a fucking retard");
		burn.add(" was born mentally disabled");	
		burn.add(" xD");	
		burn.add(" I'm a robot and I think you are mentally challenged by this world");				
}*/
    
    static void buildGifs() {
		gifs = new String[]{
				"C:\\Users\\eriks\\Desktop\\gif1.gif",
				"C:\\Users\\eriks\\Desktop\\gif2.gif",
				"C:\\Users\\eriks\\Desktop\\gif3.gif",
				"C:\\Users\\eriks\\Desktop\\gif4.gif",
		};			
	}
    static String[] swarm() {
    	String[] temp = new String[5];
    	for (int i = 0 ; i < 5; i++) {
    		temp[i] = burn.get(rand.nextInt(burn.size()));
    	}
    	return temp;
    }
}
