package ruche.sti2d.network;

import org.apache.mina.core.service.IoHandler;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.transport.socket.nio.NioSocketConnector;
import ruche.sti2d.core.MainActivity;

import java.net.InetSocketAddress;

/**
 * Created by Botan on 15/12/2016. 21:10
 */
public class Connector implements IoHandler {
    private final static int PORT = 2000;
    private final static String ADDRESS = "127.0.0.1";

    private final MainActivity mainActivity;

    private NioSocketConnector connector;

    public void attachConnectedActivity() {

    }

    public Connector(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }

    @Override
    public void sessionCreated(IoSession session) throws Exception {
        mainActivity.updateStatus(MainActivity.State.CONNECTED);
    }

    @Override
    public void sessionOpened(IoSession session) throws Exception {

    }

    @Override
    public void sessionClosed(IoSession session) throws Exception {

    }

    @Override
    public void sessionIdle(IoSession session, IdleStatus status) throws Exception {

    }

    @Override
    public void exceptionCaught(IoSession session, Throwable cause) throws Exception {

    }

    @Override
    public void messageReceived(IoSession session, Object message) throws Exception {

    }

    @Override
    public void messageSent(IoSession session, Object message) throws Exception {

    }

    @Override
    public void inputClosed(IoSession session) throws Exception {

    }

    public void connect() {
        this.connector = new NioSocketConnector();
        this.connector.setHandler(this);

        connector.setConnectTimeoutMillis(2000); //on attend 10 secondes, sinon, c'est que le serveur n'est pas allumé
        connector.connect(new InetSocketAddress(ADDRESS, PORT));

        //verification si on est connecté
        new android.os.Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!connector.isActive()) {
                    mainActivity.updateStatus(MainActivity.State.CONNECTED);
                }
            }
        }, connector.getConnectTimeoutMillis());
    }
}
