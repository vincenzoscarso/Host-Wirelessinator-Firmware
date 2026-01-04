export default {
    base: './', // Ensures paths work on GitHub Pages
    build: {
        outDir: '../docs', // Sends the build up one level into the root docs folder
        emptyOutDir: true, // Cleans the folder before building
    }
}