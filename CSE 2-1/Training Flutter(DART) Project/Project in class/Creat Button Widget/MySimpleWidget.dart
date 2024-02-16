import 'package:flutter/material.dart';

class MySimpleWidget extends StatelessWidget {
  MySimpleWidget({super.key});
  @override
  Widget build(BuildContext context) {
    return ElevatedButton(onPressed: () {}, child: Text("Press me"));
  }
}
