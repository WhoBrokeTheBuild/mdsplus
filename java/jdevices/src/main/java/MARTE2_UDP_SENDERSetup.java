/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author mdsplus
 */
public class MARTE2_UDP_SENDERSetup extends DeviceSetup {

    /**
     * Creates new form MARTE2_UDP_SENDERSetup
     */
    public MARTE2_UDP_SENDERSetup() {
        initComponents();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        deviceButtons1 = new DeviceButtons();
        jPanel1 = new javax.swing.JPanel();
        jPanel2 = new javax.swing.JPanel();
        deviceField1 = new DeviceField();
        deviceField2 = new DeviceField();
        jPanel3 = new javax.swing.JPanel();
        deviceField3 = new DeviceField();
        deviceField4 = new DeviceField();
        jScrollPane1 = new javax.swing.JScrollPane();
        jPanel4 = new javax.swing.JPanel();
        jPanel5 = new javax.swing.JPanel();
        jPanel6 = new javax.swing.JPanel();
        deviceChoice1 = new DeviceChoice();
        deviceField5 = new DeviceField();
        jPanel7 = new javax.swing.JPanel();
        deviceField6 = new DeviceField();
        jPanel8 = new javax.swing.JPanel();
        jPanel9 = new javax.swing.JPanel();
        deviceChoice2 = new DeviceChoice();
        deviceField7 = new DeviceField();
        jPanel10 = new javax.swing.JPanel();
        deviceField8 = new DeviceField();
        jPanel11 = new javax.swing.JPanel();
        jPanel12 = new javax.swing.JPanel();
        deviceChoice3 = new DeviceChoice();
        deviceField9 = new DeviceField();
        jPanel13 = new javax.swing.JPanel();
        deviceField10 = new DeviceField();
        jPanel14 = new javax.swing.JPanel();
        jPanel15 = new javax.swing.JPanel();
        deviceChoice4 = new DeviceChoice();
        deviceField11 = new DeviceField();
        jPanel16 = new javax.swing.JPanel();
        deviceField12 = new DeviceField();
        jPanel17 = new javax.swing.JPanel();
        jPanel18 = new javax.swing.JPanel();
        deviceChoice5 = new DeviceChoice();
        deviceField13 = new DeviceField();
        jPanel19 = new javax.swing.JPanel();
        deviceField14 = new DeviceField();
        jPanel20 = new javax.swing.JPanel();
        jPanel21 = new javax.swing.JPanel();
        deviceChoice6 = new DeviceChoice();
        deviceField15 = new DeviceField();
        jPanel22 = new javax.swing.JPanel();
        deviceField16 = new DeviceField();
        jPanel23 = new javax.swing.JPanel();
        jPanel24 = new javax.swing.JPanel();
        deviceChoice7 = new DeviceChoice();
        deviceField17 = new DeviceField();
        jPanel25 = new javax.swing.JPanel();
        deviceField18 = new DeviceField();
        jPanel26 = new javax.swing.JPanel();
        jPanel27 = new javax.swing.JPanel();
        deviceChoice8 = new DeviceChoice();
        deviceField19 = new DeviceField();
        jPanel28 = new javax.swing.JPanel();
        deviceField20 = new DeviceField();
        jPanel29 = new javax.swing.JPanel();
        jPanel30 = new javax.swing.JPanel();
        deviceChoice9 = new DeviceChoice();
        deviceField21 = new DeviceField();
        jPanel31 = new javax.swing.JPanel();
        deviceField22 = new DeviceField();

        setDeviceProvider("localhost:8100");
        setDeviceTitle("MARTe2 UDP Sender");
        setDeviceType("MARTE2_UDP_SENDER");
        setHeight(400);
        setWidth(600);
        getContentPane().add(deviceButtons1, java.awt.BorderLayout.PAGE_END);

        jPanel1.setLayout(new java.awt.GridLayout(2, 1));

        deviceField1.setIdentifier("");
        deviceField1.setLabelString("Address: ");
        deviceField1.setOffsetNid(7);
        deviceField1.setTextOnly(true);
        jPanel2.add(deviceField1);

        deviceField2.setIdentifier("");
        deviceField2.setLabelString("Port: ");
        deviceField2.setNumCols(5);
        deviceField2.setOffsetNid(10);
        jPanel2.add(deviceField2);

        jPanel1.add(jPanel2);

        deviceField3.setIdentifier("");
        deviceField3.setLabelString("Timebase: ");
        deviceField3.setNumCols(20);
        deviceField3.setOffsetNid(3);
        jPanel3.add(deviceField3);

        deviceField4.setIdentifier("");
        deviceField4.setLabelString("Timebase Div.: ");
        deviceField4.setNumCols(5);
        deviceField4.setOffsetNid(134);
        jPanel3.add(deviceField4);

        jPanel1.add(jPanel3);

        getContentPane().add(jPanel1, java.awt.BorderLayout.PAGE_START);

        jPanel4.setBorder(javax.swing.BorderFactory.createEmptyBorder(1, 1, 1, 1));
        jPanel4.setLayout(new java.awt.GridLayout(9, 1));

        jPanel5.setBorder(javax.swing.BorderFactory.createTitledBorder("Time"));
        jPanel5.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice1.setChoiceItems(new String[] {"uint32"});
        deviceChoice1.setIdentifier("");
        deviceChoice1.setLabelString("Type: ");
        deviceChoice1.setOffsetNid(16);
        deviceChoice1.setUpdateIdentifier("");
        jPanel6.add(deviceChoice1);

        deviceField5.setEditable(false);
        deviceField5.setIdentifier("");
        deviceField5.setLabelString("Dimensions: ");
        deviceField5.setNumCols(5);
        deviceField5.setOffsetNid(17);
        jPanel6.add(deviceField5);

        jPanel5.add(jPanel6);

        deviceField6.setIdentifier("");
        deviceField6.setLabelString("Value: ");
        deviceField6.setNumCols(25);
        deviceField6.setOffsetNid(19);
        jPanel7.add(deviceField6);

        jPanel5.add(jPanel7);

        jPanel4.add(jPanel5);

        jPanel8.setBorder(javax.swing.BorderFactory.createTitledBorder("In1"));
        jPanel8.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice2.setChoiceItems(new String[] {"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"});
        deviceChoice2.setIdentifier("");
        deviceChoice2.setLabelString("Type: ");
        deviceChoice2.setOffsetNid(23);
        deviceChoice2.setUpdateIdentifier("");
        jPanel9.add(deviceChoice2);

        deviceField7.setIdentifier("");
        deviceField7.setLabelString("Dimensions: ");
        deviceField7.setNumCols(5);
        deviceField7.setOffsetNid(24);
        jPanel9.add(deviceField7);

        jPanel8.add(jPanel9);

        deviceField8.setIdentifier("");
        deviceField8.setLabelString("Value: ");
        deviceField8.setNumCols(25);
        deviceField8.setOffsetNid(26);
        jPanel10.add(deviceField8);

        jPanel8.add(jPanel10);

        jPanel4.add(jPanel8);

        jPanel11.setBorder(javax.swing.BorderFactory.createTitledBorder("In2"));
        jPanel11.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice3.setChoiceItems(new String[] {"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"});
        deviceChoice3.setIdentifier("");
        deviceChoice3.setLabelString("Type: ");
        deviceChoice3.setOffsetNid(30);
        deviceChoice3.setUpdateIdentifier("");
        jPanel12.add(deviceChoice3);

        deviceField9.setIdentifier("");
        deviceField9.setLabelString("Dimensions: ");
        deviceField9.setNumCols(5);
        deviceField9.setOffsetNid(31);
        jPanel12.add(deviceField9);

        jPanel11.add(jPanel12);

        deviceField10.setIdentifier("");
        deviceField10.setLabelString("Value: ");
        deviceField10.setNumCols(25);
        deviceField10.setOffsetNid(33);
        jPanel13.add(deviceField10);

        jPanel11.add(jPanel13);

        jPanel4.add(jPanel11);

        jPanel14.setBorder(javax.swing.BorderFactory.createTitledBorder("In3"));
        jPanel14.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice4.setChoiceItems(new String[] {"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"});
        deviceChoice4.setIdentifier("");
        deviceChoice4.setLabelString("Type: ");
        deviceChoice4.setOffsetNid(37);
        deviceChoice4.setUpdateIdentifier("");
        jPanel15.add(deviceChoice4);

        deviceField11.setIdentifier("");
        deviceField11.setLabelString("Dimensions: ");
        deviceField11.setNumCols(5);
        deviceField11.setOffsetNid(38);
        jPanel15.add(deviceField11);

        jPanel14.add(jPanel15);

        deviceField12.setIdentifier("");
        deviceField12.setLabelString("Value: ");
        deviceField12.setNumCols(25);
        deviceField12.setOffsetNid(40);
        jPanel16.add(deviceField12);

        jPanel14.add(jPanel16);

        jPanel4.add(jPanel14);

        jPanel17.setBorder(javax.swing.BorderFactory.createTitledBorder("In4"));
        jPanel17.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice5.setChoiceItems(new String[] {"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"});
        deviceChoice5.setIdentifier("");
        deviceChoice5.setLabelString("Type: ");
        deviceChoice5.setOffsetNid(44);
        deviceChoice5.setUpdateIdentifier("");
        jPanel18.add(deviceChoice5);

        deviceField13.setIdentifier("");
        deviceField13.setLabelString("Dimensions: ");
        deviceField13.setNumCols(5);
        deviceField13.setOffsetNid(45);
        jPanel18.add(deviceField13);

        jPanel17.add(jPanel18);

        deviceField14.setIdentifier("");
        deviceField14.setLabelString("Value: ");
        deviceField14.setNumCols(25);
        deviceField14.setOffsetNid(47);
        jPanel19.add(deviceField14);

        jPanel17.add(jPanel19);

        jPanel4.add(jPanel17);

        jPanel20.setBorder(javax.swing.BorderFactory.createTitledBorder("In5"));
        jPanel20.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice6.setChoiceItems(new String[] {"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"});
        deviceChoice6.setIdentifier("");
        deviceChoice6.setLabelString("Type: ");
        deviceChoice6.setOffsetNid(51);
        deviceChoice6.setUpdateIdentifier("");
        jPanel21.add(deviceChoice6);

        deviceField15.setIdentifier("");
        deviceField15.setLabelString("Dimensions: ");
        deviceField15.setNumCols(5);
        deviceField15.setOffsetNid(52);
        jPanel21.add(deviceField15);

        jPanel20.add(jPanel21);

        deviceField16.setIdentifier("");
        deviceField16.setLabelString("Value: ");
        deviceField16.setNumCols(25);
        deviceField16.setOffsetNid(54);
        jPanel22.add(deviceField16);

        jPanel20.add(jPanel22);

        jPanel4.add(jPanel20);

        jPanel23.setBorder(javax.swing.BorderFactory.createTitledBorder("In6"));
        jPanel23.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice7.setChoiceItems(new String[] {"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"});
        deviceChoice7.setIdentifier("");
        deviceChoice7.setLabelString("Type: ");
        deviceChoice7.setOffsetNid(58);
        deviceChoice7.setUpdateIdentifier("");
        jPanel24.add(deviceChoice7);

        deviceField17.setIdentifier("");
        deviceField17.setLabelString("Dimensions: ");
        deviceField17.setNumCols(5);
        deviceField17.setOffsetNid(59);
        jPanel24.add(deviceField17);

        jPanel23.add(jPanel24);

        deviceField18.setIdentifier("");
        deviceField18.setLabelString("Value: ");
        deviceField18.setNumCols(25);
        deviceField18.setOffsetNid(61);
        jPanel25.add(deviceField18);

        jPanel23.add(jPanel25);

        jPanel4.add(jPanel23);

        jPanel26.setBorder(javax.swing.BorderFactory.createTitledBorder("In7"));
        jPanel26.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice8.setChoiceItems(new String[] {"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"});
        deviceChoice8.setIdentifier("");
        deviceChoice8.setLabelString("Type: ");
        deviceChoice8.setOffsetNid(65);
        deviceChoice8.setUpdateIdentifier("");
        jPanel27.add(deviceChoice8);

        deviceField19.setIdentifier("");
        deviceField19.setLabelString("Dimensions: ");
        deviceField19.setNumCols(5);
        deviceField19.setOffsetNid(66);
        jPanel27.add(deviceField19);

        jPanel26.add(jPanel27);

        deviceField20.setIdentifier("");
        deviceField20.setLabelString("Value: ");
        deviceField20.setNumCols(25);
        deviceField20.setOffsetNid(68);
        jPanel28.add(deviceField20);

        jPanel26.add(jPanel28);

        jPanel4.add(jPanel26);

        jPanel29.setBorder(javax.swing.BorderFactory.createTitledBorder("In8"));
        jPanel29.setLayout(new java.awt.GridLayout(2, 1));

        deviceChoice9.setChoiceItems(new String[] {"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "float32", "float64"});
        deviceChoice9.setIdentifier("");
        deviceChoice9.setLabelString("Type: ");
        deviceChoice9.setOffsetNid(72);
        deviceChoice9.setUpdateIdentifier("");
        jPanel30.add(deviceChoice9);

        deviceField21.setIdentifier("");
        deviceField21.setLabelString("Dimensions: ");
        deviceField21.setNumCols(5);
        deviceField21.setOffsetNid(73);
        jPanel30.add(deviceField21);

        jPanel29.add(jPanel30);

        deviceField22.setIdentifier("");
        deviceField22.setLabelString("Value: ");
        deviceField22.setNumCols(25);
        deviceField22.setOffsetNid(75);
        jPanel31.add(deviceField22);

        jPanel29.add(jPanel31);

        jPanel4.add(jPanel29);

        jScrollPane1.setViewportView(jPanel4);

        getContentPane().add(jScrollPane1, java.awt.BorderLayout.CENTER);
    }// </editor-fold>//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private DeviceButtons deviceButtons1;
    private DeviceChoice deviceChoice1;
    private DeviceChoice deviceChoice2;
    private DeviceChoice deviceChoice3;
    private DeviceChoice deviceChoice4;
    private DeviceChoice deviceChoice5;
    private DeviceChoice deviceChoice6;
    private DeviceChoice deviceChoice7;
    private DeviceChoice deviceChoice8;
    private DeviceChoice deviceChoice9;
    private DeviceField deviceField1;
    private DeviceField deviceField10;
    private DeviceField deviceField11;
    private DeviceField deviceField12;
    private DeviceField deviceField13;
    private DeviceField deviceField14;
    private DeviceField deviceField15;
    private DeviceField deviceField16;
    private DeviceField deviceField17;
    private DeviceField deviceField18;
    private DeviceField deviceField19;
    private DeviceField deviceField2;
    private DeviceField deviceField20;
    private DeviceField deviceField21;
    private DeviceField deviceField22;
    private DeviceField deviceField3;
    private DeviceField deviceField4;
    private DeviceField deviceField5;
    private DeviceField deviceField6;
    private DeviceField deviceField7;
    private DeviceField deviceField8;
    private DeviceField deviceField9;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel10;
    private javax.swing.JPanel jPanel11;
    private javax.swing.JPanel jPanel12;
    private javax.swing.JPanel jPanel13;
    private javax.swing.JPanel jPanel14;
    private javax.swing.JPanel jPanel15;
    private javax.swing.JPanel jPanel16;
    private javax.swing.JPanel jPanel17;
    private javax.swing.JPanel jPanel18;
    private javax.swing.JPanel jPanel19;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel20;
    private javax.swing.JPanel jPanel21;
    private javax.swing.JPanel jPanel22;
    private javax.swing.JPanel jPanel23;
    private javax.swing.JPanel jPanel24;
    private javax.swing.JPanel jPanel25;
    private javax.swing.JPanel jPanel26;
    private javax.swing.JPanel jPanel27;
    private javax.swing.JPanel jPanel28;
    private javax.swing.JPanel jPanel29;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel30;
    private javax.swing.JPanel jPanel31;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JPanel jPanel7;
    private javax.swing.JPanel jPanel8;
    private javax.swing.JPanel jPanel9;
    private javax.swing.JScrollPane jScrollPane1;
    // End of variables declaration//GEN-END:variables
}
