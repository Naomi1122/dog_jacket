import { initializeApp } from "firebase/app";
import { getFirestore } from "firebase/firestore";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyDZAiahjyaU4il1wpxvhVLmQE6pYWpkCEI",
  authDomain: "doggydynamic.firebaseapp.com",
  projectId: "doggydynamic",
  storageBucket: "doggydynamic.appspot.com",
  messagingSenderId: "451258203225",
  appId: "1:451258203225:web:781f85e964b36fd5c84548",
  measurementId: "G-PQ554RVT9H"
}

// Initialize Firebase
initializeApp(firebaseConfig)
const db = getFirestore()
export default db