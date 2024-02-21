import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.white12),
        useMaterial3: true,
      ),
      home: const MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key}) : super(key: key);

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

  var wtController = TextEditingController();
  var ftController = TextEditingController();
  var inController = TextEditingController();

  var result = "";
  var bgColor = Colors.indigo.shade200;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Center(
            child: Text('Your BMI',style: TextStyle(
              fontSize: 40,color: Colors.green, fontWeight: FontWeight.w700
            ),)),
      ),
      body: Container(
        color: bgColor,
        child: Center(
          child: Container(
            width: 300,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text('BMI',style: TextStyle(
                  fontSize: 34, fontWeight: FontWeight.w700
                ),),

                SizedBox(height: 21,),

                TextField(
                  controller: wtController,
                  decoration: InputDecoration(
                    label: Text('Enter your Weight'),
                    prefixIcon: Icon(Icons.line_weight),
                  ),
                  keyboardType: TextInputType.number,
                ),

                SizedBox(height: 11,),

                TextField(
                  controller: ftController,
                  decoration: InputDecoration(
                    label: Text('Enter your Height(in Feet) '),
                    prefixIcon: Icon(Icons.height),
                  ),
                  keyboardType: TextInputType.number,
                ),

                SizedBox(height: 11,),

                TextField(
                  controller: inController,
                  decoration: InputDecoration(
                    label: Text('Enter your Height(in Inch)'),
                    prefixIcon: Icon(Icons.height),
                  ),
                  keyboardType: TextInputType.number,
                ),

                ElevatedButton(onPressed: (){

                  var wt = wtController.text;
                  var ft = ftController.text;
                  var inch = inController.text;

                  if(wt != "" && ft != "" && inch != ""){

                    //BMI Calculation

                    var iWt = int.parse(wt);
                    var iFt = int.parse(ft);
                    var iInch = int.parse(inch);

                    var tInch = (iFt*12) + iInch;

                    var tCm = tInch * 2.54;

                    var tM = tCm/100;

                    var bmi = iWt/(tM*tM);

                    var msg = "";

                    if(bmi>25){
                      msg = "You're OverWeight!";
                      bgColor = Colors.redAccent;
                    }else if(bmi<18){
                      msg = "You're UnderWeight!";
                      bgColor = Colors.orange.shade200;
                    }else{
                      msg = "You're Healthy!";
                      bgColor = Colors.green.shade200;
                    }

                    setState(() {
                      result = "$msg \nYour BMI is: ${bmi.toStringAsFixed(2)}";
                    });

                  } else{
                    setState(() {
                      result = "Please fil all the required blanks!";
                    });

                  }

                }, child: Text('Calculate')),

                 Center(child: Text(result, style: TextStyle(fontSize: 18)))

                ],
            ),
          ),
        ),
      )

    );
  }
}
