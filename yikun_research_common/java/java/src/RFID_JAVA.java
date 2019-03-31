import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;

import com.clou.uhf.G3Lib.CLReader;
import com.clou.uhf.G3Lib.Example;
import com.clou.uhf.G3Lib.ClouInterface.IAsynchronousMessage;
import com.clou.uhf.G3Lib.Enumeration.eReadType;
import com.clou.uhf.G3Lib.Protocol.Tag_Model;

public class RFID_JAVA implements IAsynchronousMessage{

	/**
	 * @param args
	 * @throws IOException 
	 */
	
	//static HashSet<String> Tags_Tid = new HashSet<String>();
	int tag_num = 0;
	HashSet<String> Tags_Tid = new HashSet<String>();
	
	public static void main(String[] args) throws IOException, InterruptedException {
		// TODO Auto-generated method stub	
		
		Scanner sc = new Scanner(System.in);
		String connID = "192.168.1.116:9090";
		sc.next();

		RFID_JAVA mc = new RFID_JAVA();
		if(CLReader.CreateTcpConn(connID, mc))
		{
			System.out.println("connection success...");
			CLReader.Stop(connID);		
		}
		else
		{
			System.out.println("connection failure!");
		}
		eReadType readType = eReadType.Inventory;
		CLReader._Tag6C.GetEPC_TID(connID, 1, readType);
		sc.next();
		
		CLReader.Stop(connID);
	}

	@Override
	public void GPIControlMsg(int arg0, int arg1, int arg2) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OutPutTags(Tag_Model model) {
		// TODO Auto-generated method stub
		// output tag
		tag_num++;
		//Tags_Tid.add(model._TID);
		//System.out.println("tags number:"+Tags_Tid.size());
		switch (model._TID) {
			case "E2801105200076526F9A0979":
			System.out.println("位置1，tag TID = "+"E2801105200076526F9A0979,序号:"+tag_num);
			break;
		case "E28011052000764D6F8E0979":
			System.out.println("位置2，tag TID = "+"E28011052000764D6F8E0979,序号:"+tag_num);
			break;
		case "E2801105200070846FA70979":
			System.out.println("位置3，tag TID = "+"E2801105200070846FA70979,序号:"+tag_num);
			break;
        case "E28011052000718D6FA80979":
			System.out.println("位置4，tag TID = "+"E28011052000718D6FA80979,序号:"+tag_num);
			break;
        case "E2801105200070946FAA0979":
			System.out.println("位置5，tag TID = "+"E2801105200070946FAA0979,序号:"+tag_num);
			break;
		default:
			//System.out.println("没有检测到标签.");
			break;
		}
		
		if(tag_num==Integer.MAX_VALUE)
			tag_num=0;
	}

	@Override
	public void OutPutTagsOver() {
		// TODO Auto-generated method stub
	}

	@Override
	public void PortClosing(String arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void PortConnecting(String arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void WriteDebugMsg(String arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void WriteLog(String arg0) {
		// TODO Auto-generated method stub
		
	}

}
